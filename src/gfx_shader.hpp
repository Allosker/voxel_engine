#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Define basic Render Shader, up and running as it should
* ==============================================-
*/

#include <string_view>

#include "sys_graphics.hpp"
#include "sys_types.hpp"


namespace gfx
{
	class Shader
	{
	public:

		// = Construction/Destruction

		Shader(const filepath& vertShader, const filepath& fragShader, const filepath& geomShader = "");

		Shader(Shader&& other) noexcept;
		Shader& operator=(Shader&& other) noexcept;

		~Shader() noexcept;


		// = Actors

		void bind() const noexcept;

		void unbind() const noexcept; 


		// = Getters

		GLuint ID() const noexcept;

		i32 getUniformLocation(std::string_view name) const noexcept;


		// = Setters

		void setValue(std::string_view name, float value) const noexcept;

		void setValue(std::string_view name, const v2f32& value) const noexcept;
		void setValue(std::string_view name, const v3f32& value) const noexcept;
		void setValue(std::string_view name, const v4f32& value) const noexcept;

		void setValue(std::string_view name, const m3f32& value) const noexcept;
		void setValue(std::string_view name, const m4f32& value) const noexcept;

		void setValueLocation(GLint location, const m4f32& value) const noexcept;


	private:

		void compile(GLuint s_id, std::string_view name);

		void link(GLuint s_id);

		GLuint m_id;

	};
}