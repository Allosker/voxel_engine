#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Define basic Render Shader, up and running as it should
* ==============================================-
*/

#include <string_view>
#include <unordered_set>

#include "sys/graphics.hpp"
#include "sys/types.hpp"
#include "sys/hash.hpp"
#include "gl.hpp"

namespace gfx
{
	struct UniformDefinition
	{
		int32_t block{};
		int32_t pos{};
		int32_t size{};
		uint32_t type{};
		int32_t textureSlot{};

		std::string name; // Debug only?
	};
	using UniformDefinitions = std::unordered_map<StringHash, UniformDefinition>;

	struct UniformBlockDefinition
	{
		//uint32_t index{};
		uint32_t totalSize{};
		uint32_t bindSlot{};

		std::string name; // Debug only?
	};

	struct GlobalUniformBlockDefinition : UniformBlockDefinition
	{
		UniformDefinitions m_uniformDefinitions;
	};

	static constexpr uint32_t MaxGlobalBlocks = 10; 

	class Shader
	{
	public:
		// = Construction/Destruction

		Shader(const filepath& vertShader, const filepath& fragShader, const filepath& geomShader = "");

		Shader(std::string_view vert, std::string_view frag, std::string_view geom = "")
		{
			init(vert, frag, geom);
		}

		Shader(const char* vert, const char* frag, const char* geom = "")
		{
			std::string v{ vert };
			std::string f{ frag };
			std::string g{ geom };
			init(v, f, g);
		}

		Shader(Shader&& other) noexcept = default;
		Shader& operator=(Shader&& other) noexcept = default;

		~Shader() noexcept;


		// = Actors

		void bind() const noexcept;

		void unbind() const noexcept;


		// = Getters

		GLuint id() const noexcept;

		GLuint get_uni_loc(std::string_view name) const noexcept;

		const std::vector<UniformBlockDefinition>& get_block_definitions() const
		{
			return m_blockDefinitions;
		}

		const std::unordered_map<StringHash, UniformDefinition>& get_uniform_definitions() const
		{
			return m_uniformDefinitions;
		}

		uint32_t get_texture_count() const
		{
			return m_textureSlotCount;
		}

		// = Setters

		void set_value(std::string_view name, float value) const noexcept;

		void set_value(std::string_view name, const v2f32& value) const noexcept;
		void set_value(std::string_view name, const v3f32& value) const noexcept;
		void set_value(std::string_view name, const v4f32& value) const noexcept;

		void set_value(std::string_view name, const m3f32& value) const noexcept;
		void set_value(std::string_view name, const m4f32& value) const noexcept;

		void set_value_loc(GLint location, const m4f32& value) const noexcept;

		static const GlobalUniformBlockDefinition* FindGlobalUniformBlockDefinition(StringHash id);

	private:

		void compile(GLuint s_id, std::string_view name);

		void link(GLuint s_id);

		void init(std::string_view vert, std::string_view frag, std::string_view geom);


		GlId_Shader m_id;

		uint32_t m_textureSlotCount{};
		std::vector<UniformBlockDefinition> m_blockDefinitions;
		UniformDefinitions m_uniformDefinitions;

		static inline std::unordered_set<StringHash> g_globalBlockNames{"ViewData", "InstanceData"};
		static inline std::unordered_map<StringHash, GlobalUniformBlockDefinition> g_globalBlockDefinitions;
	};
}