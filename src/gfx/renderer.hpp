#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
*/

#include <vector>

#include "sys/types.hpp"

#include "gfx/camera.hpp"
#include "gfx/material.hpp"

namespace gfx
{
	class Mesh;

	enum class RenderLayer : uint64_t
	{
		Opaque = 0,
		Transparent = 1,
		UI = 2,

		COUNT
	};

	struct DrawKey
	{
		DrawKey(RenderLayer layer, float depth)
		{
			value = ((uint64_t)std::to_underlying(layer) << renderLayerOffset) & renderLayerMask;

			if (layer != RenderLayer::UI)
			{
				const std::uint32_t depthBits = std::bit_cast<std::uint32_t>(std::max(0.f, depth));

				if (layer == RenderLayer::Transparent)
				{
					value |= (0xFFFFFF - depthBits);
				}
				else
				{
					value |= depthBits;
				}
			}
		}

		DrawKey(const DrawKey&) = default;
		DrawKey& operator=(const DrawKey&) = default;

		DrawKey(DrawKey&&) = default;
		DrawKey& operator=(DrawKey&&) = default;

		RenderLayer getRenderLayer() const 
		{
			return static_cast<RenderLayer>((value & renderLayerMask) >> renderLayerOffset);
		}

		bool operator<(const DrawKey& other) const
		{
			return value < other.value;
		}
	

	private:
		static constexpr uint64_t bits = 64;

		static constexpr uint64_t renderLayerBits = 4;
		static constexpr uint64_t renderLayerOffset = bits - renderLayerBits;
		static constexpr uint64_t renderLayerMask = uint64_t(0b1111) << renderLayerOffset;

		static constexpr uint64_t depthBits = 32;
		static constexpr uint64_t depthOffset = 0;
		static constexpr uint64_t depthMask = uint32_t(-1);

		uint64_t value;
	};

	struct DrawCommand
	{
		DrawKey key;
		Mesh* mesh{};
		m4f32 transform{1};
		Material* material{};

		bool operator<(const DrawCommand& other) const
		{
			return key < other.key;
		}
	};

	class Renderer
	{
	public:

		void push_command(Mesh* mesh, m4f32 transform, Material* material, RenderLayer layer, float depth)
		{
			m_commands.emplace_back(DrawCommand{
				.key = {layer, depth},
				.mesh = mesh,
				.transform = transform,
				.material = material
			});
		}

		void push_command(Mesh* mesh, m4f32 transform, Material* material, RenderLayer layer)
		{
			const v4f32 worldPos(v3f32(transform[3]), 1);
			float depth = -glm::dot(m_viewZRow, worldPos);

			m_commands.emplace_back(DrawCommand{
				.key = {layer, depth},
				.mesh = mesh,
				.transform = transform,
				.material = material
			});
		}

		void push_command(Mesh* mesh, Material* material, RenderLayer layer)
		{
			m_commands.emplace_back(DrawCommand{
				.key = {layer, 0.f},
				.mesh = mesh,
				.transform = m4f32{1},
				.material = material
			});
		}

		void push_command(Mesh* mesh, m3f32 transform, Material* material)
		{
			m_commands.emplace_back(DrawCommand{
				.key = {RenderLayer::UI, 0.f},
				.mesh = mesh,
				.transform = static_cast<m4f32>(transform),
				.material = material
			});
		}

		void push_command(Mesh* mesh, m3f32 transform, Material* material, RenderLayer layer)
		{
			m_commands.emplace_back(DrawCommand{
				.key = {layer, 0.f},
				.mesh = mesh,
				.transform = static_cast<m4f32>(transform),
				.material = material
			});
		}

		void start(const Camera& camera, m4f32 uiViewMatrix);
		void draw();

	private:
		Camera m_camera;
		m4f32 m_viewMatrix;
		glm::vec4 m_viewZRow;

		m4f32 m_uiViewMatrix;

		std::vector<DrawCommand> m_commands;

		GlobalUniformBlockInstance viewUBI{ *Shader::FindGlobalUniformBlockDefinition("ViewData") };
		GlobalUniformBlockInstance instanceUBI{ *Shader::FindGlobalUniformBlockDefinition("InstanceData") };
	};
}