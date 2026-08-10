#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Special mesh that builds itself on a chunk
* ==============================================-
*/

#include <vector>
#include <array>

#include "sys_graphics.hpp"
#include "sys_types.hpp"

#include "gfx_chunk.hpp"


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


		~ChunkMesh() noexcept;


	// = Predicates

		bool has_mesh() const noexcept { return !m_vao && !m_vbo; }


	// = Actors

		// Build then update the mesh
		void update(const Chunk& current_chunk, const ChunkGrid& grid) noexcept
		{
			update_mesh(build_mesh(current_chunk, grid));
		}

		std::vector<VoxelVertex> build_mesh(const Chunk& current_chunk, const ChunkGrid& grid) noexcept;

		void update_mesh(const std::vector<VoxelVertex>& vertices) noexcept;


		void draw() const noexcept
		{
			glBindVertexArray(m_vao);

			glDrawArrays(GL_TRIANGLES, 0, m_vertices_count);

			glBindVertexArray(0);
		}


	public:

        template<typename T>
		static constexpr std::array<std::array<mpml::Vector3<T>, 8>, 6> ao_dirs
		{
			std::array<mpml::Vector3<T>, 8>
			{
				mpml::Vector3<T>
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
				mpml::Vector3<T>
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
				mpml::Vector3<T>
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
				mpml::Vector3<T>
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
				mpml::Vector3<T>
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
				mpml::Vector3<T>
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
        static constexpr std::array<float, 4> ao_values{ 0.7f, 0.8f, 0.9f, 1.f }; 


	private:

		void create_buffers() noexcept;

		void free_resources() noexcept;


	private:
		
		GLuint m_vao;
		GLuint m_vbo;

		GLsizei m_vertices_count{};

	};

}