#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	The drawable class has-a shader, texture-context, and a pointer to mesh which it can to the buffer
* ==============================================-
*/

#include <vector>


#include "sys/graphics.hpp"

#include "camera.hpp"
#include "shader.hpp"
#include "mesh.hpp"
#include "texture.hpp"


namespace gfx
{

	struct DrawContext
	{
		m4f32 model{ 1. };
		Mesh* mesh{ nullptr };
		Texture* texture{ nullptr };
		GLenum draw_mode{ GL_TRIANGLES };

		/*DrawContext(DrawContext&&) noexcept = default;
		DrawContext(const DrawContext&) noexcept = default;

		DrawContext& operator=(DrawContext&&) noexcept = default;
		DrawContext& operator=(const DrawContext&) noexcept = default;*/
	};

	class [[deprecated("unmaintained")]] Drawable
	{
	public:


		//= Init

		Drawable(Shader&& shader, Camera* camera = nullptr, const std::vector<DrawContext>& meshes = {})
			: m_shader{ std::move(shader) }, m_camera{ camera }, m_contexts { meshes }
		{
		}


		// = Setters

		void setContexts(std::vector<DrawContext> contexts) noexcept
		{
			m_contexts = contexts;
		}


		// = Actors

		void add_context(const DrawContext& context) noexcept
		{
			m_contexts.push_back(context);
		}

		void add_context(DrawContext&& context) noexcept
		{
			m_contexts.push_back(std::move(context));
		}

		bool draw() noexcept;


	private:


		std::vector<DrawContext> m_contexts;
		Shader m_shader;
		Camera* m_camera;

	};



}