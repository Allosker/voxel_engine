#include "gfx/rectangle.hpp"

#include "debugRenderer.hpp"

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

		m_trans.set_base_size(m_tex->get_size());
	}

	void Rectangle::draw(const Shader& shader) noexcept
	{
		if (!m_tex) return; 

		shader.set_value("model", m_trans.get_transform());


		m_tex->bind();

			m_mesh.draw();
		
		m_tex->unbind();
	}


}
