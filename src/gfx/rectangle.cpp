#include "gfx/rectangle.hpp"

#include "gfx/renderer.hpp"


namespace gfx
{

	void Rectangle::update_sprite(const Texture* tex) noexcept
	{
		m_material.set("tex"_id, tex);

		if (!tex)
		{
			m_mesh.update_buffer<Vertex2D>({}, {}, GL_STATIC_DRAW);
			return;
		}

		m_mesh.update_buffer<Vertex2D>(
			{
				{ { tex->get_size().x, -tex->get_size().y }, { 1, 1 } },
				{ {-tex->get_size().x, -tex->get_size().y }, { 0, 1 } },
				{ {-tex->get_size().x,  tex->get_size().y }, { 0, 0 } },
				{ { tex->get_size().x,  tex->get_size().y }, { 1, 0 } },		
			},
			{
				0, 1, 2,
				0, 2, 3
			},
			GL_STATIC_DRAW
		);

		set_size(tex->get_size());
	}

	void Rectangle::draw(Renderer& renderer) noexcept
	{
		renderer.push_command(&m_mesh, get_transform(), &m_material, RenderLayer::UI);
	}
}
