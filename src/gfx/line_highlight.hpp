#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Simplistic mesh that contains lines of a specific color to highlight an object
* ==============================================-
*/

#include "drawable.hpp"
#include "gfx/material.hpp"
#include "gfx/mesh.hpp"
#include "gfx/transformable3D.hpp"
#include "renderer.hpp"
#include "sys/assetsManager.hpp"
#include "sys/hash.hpp"
#include "vertices.hpp"
#include "line.hpp"
#include "phy/hitboxAABB.hpp"


namespace gfx
{

	class LineHighlight
		: public Transformable3D, public Drawable
	{
	public:

		LineHighlight(const v4f32& color = { 0.f, 0.f, 0.f, 1.f }, f32 width = { 1.f }) noexcept
			: m_material{ &AssetsManager::get().shaders.at("shaders/line"_id) }, m_color{ color }, m_width{ width }
		{
			m_mesh.create_buffer<Line::LineVertex>(false);
		}

		LineHighlight(const phy::HitboxAABB& hitbox, const v4f32& color = { 0.f, 0.f, 0.f, 1.f }, f32 width = { 1.f }) noexcept
			: LineHighlight{ color, width }
		{
			construct(hitbox);
		}

		DELETE_COPY_INIT(LineHighlight);
		DEFAULT_MOVE_INIT(LineHighlight);
		

		void construct(const phy::HitboxAABB& hitbox) noexcept;


		void set_width(f32 width) noexcept { m_width = width; }
		void set_color(const v4f32& color) noexcept { m_color = color; }

		void toggle_visibility() noexcept { m_draw = !m_draw; }
		

		void draw(Renderer& renderer) noexcept
		{
			if (!m_draw) return;

			m_material.get_shader().bind();
			m_material.get_shader().set_value("line_width", m_width);
			m_material.get_shader().unbind();

			renderer.push_command(&m_mesh, (m4f32)get_transform(), &m_material, RenderLayer::Opaque, (GLenum)GL_LINES);
		}


	private:

		Material m_material;

		Mesh m_mesh;

		v4f32 m_color{ 0.f, 0.f, 0.f, 1.f };
		f32 m_width{ 1.f };

		bool m_draw{ false };


	};

}