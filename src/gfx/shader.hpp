#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Define basic Render Shader, up and running as it should
* ==============================================-
*/

#include <string_view>

#include "sys/graphics.hpp"
#include "sys/types.hpp"
#include "sys/hash.hpp"


namespace gfx
{
	class Shader
	{
	public:

		// = Construction/Destruction

		Shader(const filepath& vertShader, const filepath& fragShader, const filepath& geomShader = "") noexcept;

		Shader(std::string_view vert, std::string_view frag, std::string_view geom = "") noexcept
		{
			init(vert, frag, geom);
		}

		Shader(const char* vert, const char* frag, const char* geom = "") noexcept
		{
			std::string v{ vert };
			std::string f{ frag };
			std::string g{ geom };
			init(v, f, g);
		}

		Shader(Shader&& other) noexcept;
		Shader& operator=(Shader&& other) noexcept;

		~Shader() noexcept;


		// = Actors

		void bind() const noexcept;

		void unbind() const noexcept; 


		// = Getters

		GLuint id() const noexcept;

		GLuint get_uni_loc(std::string_view name) const noexcept;


		// = Setters

		void set_value(std::string_view name, float value) const noexcept;

		void set_value(std::string_view name, const v2f32& value) const noexcept;
		void set_value(std::string_view name, const v3f32& value) const noexcept;
		void set_value(std::string_view name, const v4f32& value) const noexcept;

		void set_value(std::string_view name, const m3f32& value) const noexcept;
		void set_value(std::string_view name, const m4f32& value) const noexcept;

		void set_value_loc(GLint location, const m4f32& value) const noexcept;


	private:

		void compile(GLuint s_id, std::string_view name);

		void link(GLuint s_id);

		void init(std::string_view vert, std::string_view frag, std::string_view geom) noexcept;


		GLuint m_id;

		struct UniformDefinition
		{
			int32_t block{};
			int32_t pos{};
			int32_t size{};
			uint32_t type{};

			std::string name; // Debug only?
		};
		 
		struct BlockDefinition
		{
			uint32_t totalSize{};
			uint32_t bindIndex{};

			std::string name; // Debug only?
		};

		struct TextureMember
		{
			int32_t uniformLocation{};
			int32_t slot{};
		};

		std::vector<BlockDefinition> m_blockDefinitions;
		std::unordered_map<StringHash, UniformDefinition> m_uniformDefinitions;
		std::unordered_map<StringHash, TextureMember> m_textureMembers;

	};
}