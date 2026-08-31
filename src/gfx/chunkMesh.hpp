#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Special mesh that builds itself on a chunk
* ==============================================-
*/

#include <vector>
#include <array>

#include "sys/graphics.hpp"
#include "sys/types.hpp"

#include "chunk.hpp"


namespace gfx
{
	class ChunkGrid;


	class ChunkMesh
	{
	public:

		struct VoxelVertex
		{
			v3f32 position;
			v2f32 uvs;
			f32 ao;
		};

	public:


	// = Init

		ChunkMesh() noexcept = default;

		explicit ChunkMesh(const Chunk& current_chunk, const ChunkGrid& grid) noexcept;


		ChunkMesh(ChunkMesh&& other) noexcept;
		ChunkMesh& operator=(ChunkMesh&& other) noexcept;

		DELETE_COPY_INIT(ChunkMesh);


		~ChunkMesh() noexcept
		{
			free_resources();
		}

		/// <summary>
		/// Return whether the current mesh has a buffer
		/// </summary>
		bool has_buffer() const noexcept { return !m_vao && !m_vbo; }

		/// <summary>
		/// Bake then update the mesh to the chunk mesh buffer
		/// </summary>
		/// <param name="The chunk to build the mesh from"></param>
		/// <param name="The chunk grid it belongs to"></param>
		void update(const Chunk& current_chunk, const ChunkGrid& grid) noexcept
		{
			update_mesh(bake_mesh(current_chunk, grid));
		}

		/// <summary>
		/// Bake a mesh from a chunk: Naive Meshing + Ambient Occlusion
		/// </summary>
		/// <param name="The chunk to build the mesh from"></param>
		/// <param name="The chunk grid it belongs to"></param>
		/// <returns>The newly baked mesh as Voxel Vertices</returns>
		std::vector<VoxelVertex> bake_mesh(const Chunk& current_chunk, const ChunkGrid& grid) noexcept;

		/// <summary>
		/// Update the baked mesh passed in to the chunk mesh buffer
		/// </summary>
		/// <param name="Newly baked mesh"></param>
		void update_mesh(const std::vector<VoxelVertex>& vertices) noexcept;

		/// <summary>
		/// Draw the mesh buffer
		/// </summary>
		void draw() const noexcept
		{
			glBindVertexArray(m_vao);

			glDrawArrays(GL_TRIANGLES, 0, m_vertices_count);

			glBindVertexArray(0);
		}


	public:

        template<typename T>
		static constexpr std::array<std::array<glm::vec<3, T>, 8>, 6> ao_dirs
		{
			std::array<glm::vec<3, T>, 8>
			{
				glm::vec<3, T>
				{ 0,  1,  0 },
				{ 0,  1,  1 },
				{ 0,  0,  1 },
				{ 0, -1,  1 },
				{ 0, -1,  0 },
				{ 0, -1, -1 },
				{ 0,  0, -1 },
				{ 0,  1, -1 }
			},
			{
				glm::vec<3, T>
				{ 0, -1,  0 },
				{ 0, -1, -1 },
				{ 0,  0, -1 },
				{ 0,  1, -1 },
				{ 0,  1,  0 },
				{ 0,  1,  1 },
				{ 0,  0,  1 },
				{ 0, -1,  1 }
			},			    
			{		
				glm::vec<3, T>
				{ 1,  0,  0 },
				{ 1,  0,  1 },
				{ 0,  0,  1 },
				{-1,  0,  1 },
				{-1,  0,  0 },
				{-1,  0, -1 },
				{ 0,  0, -1 },
				{ 1,  0, -1 }
			},			    
			{		
				glm::vec<3, T>
				{-1,  0,  0 },
				{-1,  0, -1 },
				{ 0,  0, -1 },
				{ 1,  0, -1 },
				{ 1,  0,  0 },
				{ 1,  0,  1 },
				{ 0,  0,  1 },
				{-1,  0,  1 }
			},
			{
				glm::vec<3, T>
				{ 1,  0, 0 },
				{ 1,  1, 0 },
				{ 0,  1, 0 },
				{-1,  1, 0 },
				{-1,  0, 0 },
				{-1, -1, 0 },
				{ 0, -1, 0 },
				{ 1, -1, 0 }
			},
			{
				glm::vec<3, T>
				{-1,  0, 0 },
				{-1, -1, 0 },
				{ 0, -1, 0 },
				{ 1, -1, 0 },
				{ 1,  0, 0 },
				{ 1,  1, 0 },
				{ 0,  1, 0 },
				{-1,  1, 0 }
			},
			
        };

        // Ambient Occlusion values in range [0;1] representing each stage (right to left)
        static constexpr std::array<f32, 4> ao_values{ 0.85f, 0.9f, 0.95f, 1.f }; 

		bool queued{};


	private:

		/// <summary>
		/// Create vao/vbo buffers if they do not exist already
		/// </summary>
		void create_buffers() noexcept;

		/// <summary>
		/// Free allocated GPU resources
		/// </summary>
		void free_resources() noexcept;


	private:
		
		GLuint m_vao;
		GLuint m_vbo;

		GLsizei m_vertices_count{};


	};

}