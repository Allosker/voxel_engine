#include "mesh.hpp"


namespace gfx
{

	// =====================
	// Construction/Destruction
	// =====================

	Mesh::Mesh(Mesh&& other) noexcept
		: m_vao{ other.m_vao }
		, m_vbo{ other.m_vbo }
		, m_ebo{ other.m_ebo }
		, m_nb_indices{ other.m_nb_indices }
		, m_nb_elements{ other.m_nb_elements }
	{
		other.m_vao = 0;
		other.m_vbo = 0;
		other.m_ebo = 0;
		other.m_nb_indices = 0;
		other.m_nb_elements = 0;
	}

	Mesh& Mesh::operator=(Mesh&& other) noexcept
	{
		if (this == &other)
			return *this;

		free();

		m_vao = other.m_vao;
		m_vbo = other.m_vbo;
		m_ebo = other.m_ebo;
		m_nb_indices = other.m_nb_indices;
		m_nb_elements = other.m_nb_elements;

		other.m_vao = 0;
		other.m_vbo = 0;
		other.m_ebo = 0;
		other.m_nb_indices = 0;
		other.m_nb_elements = 0;

		return *this;
	}

	Mesh::~Mesh() noexcept
	{
		free();
	}


	// =====================
	// Actors 
	// =====================

	void Mesh::draw(GLenum mode) const noexcept
	{
		bind();

		if (m_ebo)
			glDrawElements(mode, m_nb_indices, GL_UNSIGNED_INT, 0);
		else
			glDrawArrays(mode, 0, m_nb_elements);

		unbind();
	}

	void Mesh::free() const noexcept
	{
		glDeleteBuffers(1, &m_vbo);
		glDeleteBuffers(1, &m_ebo);
		glDeleteVertexArrays(1, &m_vao);
	}

}
