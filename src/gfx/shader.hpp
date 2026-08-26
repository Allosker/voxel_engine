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

	};
}