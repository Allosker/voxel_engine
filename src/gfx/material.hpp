#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
*/

#include "gfx/shader.hpp"


namespace gfx
{
	class Texture;

	enum class RenderSlot
	{
		Scene,

		Dynamic_Start,
	};

	struct UniformBlockInstance
	{
		UniformBlockInstance(const UniformBlockDefinition& def);

		UniformBlockInstance(UniformBlockInstance&& other) noexcept = default;
		UniformBlockInstance& operator=(UniformBlockInstance&& other) noexcept  = default;

		void update();
		void bind() const;

		template<typename T>
		void set(const UniformDefinition& def, const T& value) noexcept
		{
			assert(def.pos + sizeof(T) <= data.size());

			std::memcpy(data.data() + def.pos, &value, sizeof(T));

			isDirty = true;
		}

	private:
		GlId_UBO id;
		uint32_t bindSlot{};
		std::vector<char> data;		
		mutable bool isDirty = true;
	};

	struct GlobalUniformBlockInstance : public UniformBlockInstance
	{
		GlobalUniformBlockInstance(const GlobalUniformBlockDefinition& def) : UniformBlockInstance(def)
		{
			defPtr = &def;
		}

		template<typename T>
		void set(StringHash name, const T& value) noexcept
		{
			auto it = defPtr->m_uniformDefinitions.find(name);
			assert(it != defPtr->m_uniformDefinitions.end());

			const UniformDefinition& def = it->second;
			UniformBlockInstance::set(def, value);
		}

		private: 

		const GlobalUniformBlockDefinition* defPtr{};
	};
	
	class Material
	{
	public:
		Material(Shader* shader) noexcept;

		Material(Material&& other) noexcept = default;
		Material& operator=(Material&& other) noexcept = default;

		template<typename T>
		void set(StringHash name, const T& value) noexcept
		{
			auto it = m_shader->get_uniform_definitions().find(name);
			if (it != m_shader->get_uniform_definitions().end())
			{
				const UniformDefinition& def = it->second;
				if (def.block >= 0)
				{
					auto& block = m_blocks[def.block];
					block.set(def, value);
				}
				else
				{
					if constexpr (std::is_same_v<T, Texture*>)
					{
						assert(def.type == GL_SAMPLER_2D || def.type == GL_SAMPLER_3D);
						m_textures[def.textureSlot] = value;
					}
					else
					{
						assert(false && "wrong type passed");
					}
				}
			}
			else
			{
				assert(false);
			}
		}

		Shader& get_shader() const
		{
			return *m_shader;
		}

		void updateBlocks() noexcept;

		void bindTextures() const noexcept;
		void bindBlocks() const noexcept;

	private:

		std::vector<UniformBlockInstance> m_blocks;
		std::vector<Texture*> m_textures;

		Shader* m_shader;
	};
}