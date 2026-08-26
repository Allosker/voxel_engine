#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Define a simple Mesh -- a GPU buffer possessing both a VAO and VBO
* This file depends on the content of "vertices.hpp"
* ==============================================-
*/


#include <vector>
#include <cassert>

#include "sys/graphics.hpp"
#include "vertices.hpp"


namespace gfx
{

	// Only accepts predefined vertices
	class Mesh
	{
	public:


		// = Destruction/Creation

		template<typename T>
		Mesh(const std::vector<T>& vertices, const std::vector<GLuint>& indices, GLenum draw_mode = GL_STATIC_DRAW)
		{
			create_buffer<T>(vertices, indices, draw_mode);
		}

		Mesh() = default;


		Mesh(const Mesh&) = delete;
		Mesh(Mesh&& other) noexcept;

		Mesh& operator=(const Mesh&) = delete;
		Mesh& operator=(Mesh&& other) noexcept;

		virtual ~Mesh() noexcept;


		// = Getters

		GLsizei getNumberIndices() const noexcept { return m_nbIndices; }


		// = Actors
		
		virtual void draw(GLenum mode = GL_TRIANGLES) const noexcept;

		// Only binds VAO/EBO
		void bind() const noexcept
		{
			glBindVertexArray(m_vao);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		}

		// Only unbinds VAO/EBO
		void unbind() const noexcept
		{
			glBindVertexArray(0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		}

		template<typename T>
		// A buffer must exist
		void update_buffer(const std::vector<T>& vertices, const std::vector<GLuint>& indices, GLenum draw_mode = GL_STATIC_DRAW) noexcept;

		template<typename T>
		// If called while a buffer already exists, erases the ancient one and creates a new one instead
		void create_buffer() noexcept;


	private:
		
		GLuint m_vao{};
		GLuint m_vbo{};
		GLuint m_ebo{};

		GLsizei m_nbIndices{};


	};


	// Actors

	template<typename T>
	inline void Mesh::update_buffer(const std::vector<T>& vertices, const std::vector<GLuint>& indices, GLenum draw_mode) noexcept
	{
		static_assert((std::is_same_v<T, Vertex> || std::is_same_v<T, Vertex2D> || std::is_same_v<T, VertexRGBA>) && "ERROR::MESH::CREATING_BUFFER::Type is not a predefined vertex one");
		assert((m_vao || m_vbo) && "ERROR::MESH::UPDATING_BUFFER::No VAO/VBO exist for the current mesh");

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		

		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(vertices.size()) * sizeof(T), vertices.data(), draw_mode);


		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

		m_nbIndices = static_cast<GLsizei>(indices.size());
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_nbIndices * sizeof(GLuint), indices.data(), draw_mode);


		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	template<typename T>
	inline void Mesh::create_buffer() noexcept
	{
		static_assert((std::is_same_v<T, Vertex> || std::is_same_v<T, Vertex2D> || std::is_same_v<T, VertexRGBA>) && "ERROR::MESH::CREATING_BUFFER::Type is not a predefined vertex one");
		if (m_vao || m_ebo || m_vao)
		{
			glDeleteBuffers(GL_ELEMENT_ARRAY_BUFFER, &m_ebo);
			glDeleteBuffers(GL_ARRAY_BUFFER, &m_vbo);	
			glDeleteVertexArrays(1, &m_vao);
		}

		glCreateVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ebo);


		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);


		glVertexAttribPointer(0, decltype(T::pos)::size, GL_FLOAT, false, sizeof(T), std::bit_cast<void*>(offsetof(T, pos)));
		glEnableVertexAttribArray(0);

		if constexpr (std::is_same_v<T, VertexRGBA>)
			glVertexAttribPointer(1, decltype(VertexRGBA::color)::size, GL_FLOAT, false, sizeof(T), std::bit_cast<void*>(offsetof(T, color)));
		else
			glVertexAttribPointer(1, decltype(T::uvs)::size, GL_FLOAT, false, sizeof(T), std::bit_cast<void*>(offsetof(T, uvs)));
		glEnableVertexAttribArray(1);


		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}


} // gfx