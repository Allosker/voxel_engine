#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Implement basic 3D lines (for now)
* ==============================================-
*/

#include "drawable.hpp"
#include "gfx/material.hpp"
#include "gfx/mesh.hpp"
#include "renderer.hpp"
#include "sys/assetsManager.hpp"
#include "sys/hash.hpp"
#include "vertices.hpp"


namespace gfx
{

	class Line
		: public Drawable
	{
	public:

		struct LineVertex
		{
			v3f32 pos;
			v4f32 rgba;

			static void setupAttributes()
			{
				DEFINE_VERTEX_VAR(0, LineVertex, pos);
				DEFINE_VERTEX_VAR(1, LineVertex, rgba);
			}
		};


	public:

		Line() noexcept
			: m_material{ &AssetsManager::get().shaders.at("shaders/line"_id) }
		{
			m_mesh.create_buffer<LineVertex>(false);
		}

		DELETE_COPY_INIT(Line);
		DEFAULT_MOVE_INIT(Line);


		void set_width(f32 width) noexcept
		{
			m_width = width;
		}

		void set_pos(const types::pos& start, const types::pos& end) noexcept
		{
			m_start = start;
			m_end = end;
			update_buffer();
		}

		void set_color(const v4f32& start_color, const v4f32& end_color = {}) noexcept
		{
			m_start_color = start_color;
			if (end_color != v4f32{})
				m_end_color = m_start_color;
			update_buffer();
		}


		const types::pos& get_start() const noexcept { return m_start; }
		const types::pos& get_end() const noexcept { return m_end; }

		const v4f32& get_start_color() const noexcept { return m_start_color; }
		const v4f32& get_end_color() const noexcept { return m_end_color; }

		f32 get_width() const noexcept { return m_width; }


		void draw(Renderer& renderer) noexcept override
		{
			m_material.get_shader().bind();
			m_material.get_shader().set_value("line_width", m_width);
			m_material.get_shader().unbind();
			renderer.push_command(&m_mesh, m4f32{ 1.f }, &m_material, RenderLayer::Opaque, (GLenum)GL_LINES);
		}


	private:

		void update_buffer() noexcept
		{
			m_mesh.update_buffer<LineVertex>(
				{
					{ static_cast<v3f32>(m_start), m_start_color },
					{ static_cast<v3f32>(m_end), m_end_color }
				});
		}


	private:

		Material m_material;

		Mesh m_mesh;

		types::pos m_start;
		types::pos m_end;
		v4f32 m_start_color{ 0.f, 0.f, 0.f, 1.f };
		v4f32 m_end_color{ 0.f, 0.f, 0.f, 1.f };
		f32 m_width{ 1.f };




	};


}