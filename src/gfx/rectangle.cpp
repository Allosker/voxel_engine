#include "gfx/rectangle.hpp"


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
				{ {-m_tex->getSize().x, -m_tex->getSize().y }, {0, 0}},
				{ { m_tex->getSize().x, -m_tex->getSize().y }, { 1, 0 } },
				{ { m_tex->getSize().x,  m_tex->getSize().y }, { 1, 1 } },
				{ {-m_tex->getSize().x,  m_tex->getSize().y }, { 0, 1 } }
			},
		{
			0, 1, 2,
			0, 2, 3
		},
			GL_STATIC_DRAW
		);

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
