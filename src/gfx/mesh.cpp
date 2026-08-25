#include "mesh.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace gfx
{

	// =====================
	// Construction/Destruction
	// =====================

	Mesh::Mesh(Mesh&& other) noexcept
		: m_vao{ other.m_vao }
		, m_vbo{ other.m_vbo }
		, m_ebo{ other.m_ebo }
		, m_nbIndices{ other.m_nbIndices }
	{
		other.m_vao = 0;
		other.m_vbo = 0;
		other.m_ebo = 0;
	}

	Mesh& Mesh::operator=(Mesh&& other) noexcept
	{
		if (this == &other)
			return *this;

		glDeleteBuffers(1, &m_vbo);
		glDeleteBuffers(1, &m_ebo);
		glDeleteVertexArrays(1, &m_vao);

		m_vao = other.m_vao;
		m_vbo = other.m_vbo;
		m_ebo = other.m_ebo;
		m_nbIndices = other.m_nbIndices;

		other.m_vao = 0;
		other.m_vbo = 0;
		other.m_ebo = 0;
		other.m_nbIndices = 0;

		return *this;
	}

	Mesh::~Mesh() noexcept
	{
		glDeleteBuffers(1, &m_vbo);
		glDeleteBuffers(1, &m_ebo);
		glDeleteVertexArrays(1, &m_vao);
	}


	// =====================
	// Actors 
	// =====================

	void Mesh::draw(GLenum mode) const noexcept
	{
		bind();

		glDrawElements(mode, m_nbIndices, GL_UNSIGNED_INT, 0);

		unbind();
	}

	void Mesh::draw_transparent(GLenum mode) const noexcept
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		bind();

		glDrawElements(mode, m_nbIndices, GL_UNSIGNED_INT, 0);

		unbind();

		glDisable(GL_BLEND);
	}

	std::optional<gfx::Mesh> Mesh::load_from_file(const filepath& path)
	{
		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(path.string(), aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			return std::nullopt;
		}
		directory = path.substr(0, path.find_last_of('/'));

		processNode(scene->mRootNode, scene);
	}

}
