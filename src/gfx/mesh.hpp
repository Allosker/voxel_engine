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

		GLsizei get_nb_indices() const noexcept { return m_nb_indices; }


		// = Actors
		
		virtual void draw(GLenum mode = GL_TRIANGLES) const noexcept;

		// Only binds VAO/EBO
		void bind() const noexcept
		{
			glBindVertexArray(m_vao);
			if (m_ebo)
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		}

		// Only unbinds VAO/EBO
		void unbind() const noexcept
		{
			glBindVertexArray(0);
			if (m_ebo)
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		}

		template<typename T>
		// A buffer must exist
		void update_buffer(const std::vector<T>& vertices, const std::vector<GLuint>& indices, GLenum draw_mode = GL_STATIC_DRAW) noexcept;

		template<typename T>
		// A buffer must exist
		void update_buffer(const std::vector<T>& vertices, GLenum draw_mode = GL_STATIC_DRAW) noexcept;

		template<typename T>
		// If called while a buffer already exists, erases the ancient one and creates a new one instead
		void create_buffer(bool create_ebo = true) noexcept;


	private:

		void free() const noexcept;
		
		GLuint m_vao{};
		GLuint m_vbo{};
		GLuint m_ebo{};

		GLsizei m_nb_indices {};
		GLsizei m_nb_elements{};


	};


	// Actors

	template<typename T>
	inline void Mesh::update_buffer(const std::vector<T>& vertices, const std::vector<GLuint>& indices, GLenum draw_mode) noexcept
	{
		static_assert((std::is_same_v<T, Vertex> || std::is_same_v<T, Vertex2D> || std::is_same_v<T, VertexRGBA> || std::is_same_v<T, Vertex2DRGBA>) 
					  && "ERROR::MESH::CREATING_BUFFER::Type is not a predefined vertex one");
		assert((m_vao || m_vbo || m_ebo) && "ERROR::MESH::UPDATING_BUFFER::No VAO/VBO/EBO exist for the current mesh");

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		

		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(vertices.size()) * sizeof(T), vertices.data(), draw_mode);


		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

		m_nb_indices = static_cast<GLsizei>(indices.size());
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_nb_indices * sizeof(GLuint), indices.data(), draw_mode);


		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	template<typename T>
	inline void Mesh::update_buffer(const std::vector<T>& vertices, GLenum draw_mode) noexcept
	{
		static_assert((std::is_same_v<T, Vertex> || std::is_same_v<T, Vertex2D> || std::is_same_v<T, VertexRGBA> || std::is_same_v<T, Vertex2DRGBA>)
					  && "ERROR::MESH::CREATING_BUFFER::Type is not a predefined vertex one");
		assert((m_vao || m_vbo) && "ERROR::MESH::UPDATING_BUFFER::No VAO/VBO exist for the current mesh");
		assert(!m_ebo && "ERROR::MESH::UPDATING_BUFFER::An EBO exists and the current mesh does not support it");

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		m_nb_elements = static_cast<GLsizei>(vertices.size());
		glBufferData(GL_ARRAY_BUFFER, m_nb_elements * sizeof(T), vertices.data(), draw_mode);


		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	template<typename T>
	inline void Mesh::create_buffer(bool create_ebo) noexcept
	{
		static_assert((std::is_same_v<T, Vertex> || std::is_same_v<T, Vertex2D> || std::is_same_v<T, VertexRGBA> || std::is_same_v<T, Vertex2DRGBA>) 
					  && "ERROR::MESH::CREATING_BUFFER::Type is not a predefined vertex one");
		if (m_vao || m_ebo || m_vao)
		{
			glDeleteBuffers(GL_ELEMENT_ARRAY_BUFFER, &m_ebo);
			glDeleteBuffers(GL_ARRAY_BUFFER, &m_vbo);	
			glDeleteVertexArrays(1, &m_vao);
		}

		glCreateVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		if (create_ebo)
			glGenBuffers(1, &m_ebo);


		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);


		glVertexAttribPointer(0, decltype(T::pos)::size, GL_FLOAT, false, sizeof(T), std::bit_cast<void*>(offsetof(T, pos)));
		glEnableVertexAttribArray(0);

		if constexpr (std::is_same_v<T, VertexRGBA> || std::is_same_v<T, Vertex2DRGBA>)
			glVertexAttribPointer(1, decltype(T::color)::size, GL_FLOAT, false, sizeof(T), std::bit_cast<void*>(offsetof(T, color)));
		else
			glVertexAttribPointer(1, decltype(T::uvs)::size, GL_FLOAT, false, sizeof(T), std::bit_cast<void*>(offsetof(T, uvs)));
		glEnableVertexAttribArray(1);


		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}


} // gfx