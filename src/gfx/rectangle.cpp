#include "gfx/rectangle.hpp"

#include "gfx/renderContext.hpp"


namespace gfx
{

	void Rectangle::update_sprite(const Texture* tex) noexcept
	{
		m_tex = tex;

		if (!tex)
		{
			m_mesh.update_buffer<Vertex2D>({}, {}, GL_STATIC_DRAW);
			return;
		}


		m_mesh.update_buffer<Vertex2D>(
			{
				{ {-m_tex->get_size().x, -m_tex->get_size().y }, { 0, 0 } },
				{ { m_tex->get_size().x, -m_tex->get_size().y }, { 1, 0 } },
				{ { m_tex->get_size().x,  m_tex->get_size().y }, { 1, 1 } },
				{ {-m_tex->get_size().x,  m_tex->get_size().y }, { 0, 1 } }
			},
			{
				0, 1, 2,
				0, 2, 3
			},
			GL_STATIC_DRAW
		);

		set_size(m_tex->get_size());
	}

	void Rectangle::draw(const RenderContext& rc) noexcept
	{
		if (!m_tex) return;

		rc.sha->set_value("model", get_transform());


		m_tex->bind();

		m_mesh.draw();

		m_tex->unbind();
	}


}
