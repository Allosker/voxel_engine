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
#include "drawable.hpp"
#include "renderer.hpp"
#include "mesh.hpp"
#include "material.hpp"


namespace gfx
{
	class ChunkGrid;


	class ChunkMesh : public Drawable
	{
	public:

		struct VoxelVertex
		{
			v3f32 position;
			v2f32 uvs;
			f32 ao;

			static void setupAttributes()
			{
				DEFINE_VERTEX_VAR(0, VoxelVertex, position);
				DEFINE_VERTEX_VAR(1, VoxelVertex, uvs);
				DEFINE_VERTEX_VAR(2, VoxelVertex, ao);
			}
		};

	public:


	// = Init

		ChunkMesh() noexcept = default;

		explicit ChunkMesh(const Chunk& current_chunk, const ChunkGrid& grid) noexcept;

		ChunkMesh(ChunkMesh&& other) noexcept = default;
		ChunkMesh& operator=(ChunkMesh&& other) noexcept = default;


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
		void draw(Renderer& renderer) override
		{
			renderer.push_command(&mesh, &material, RenderLayer::Opaque);
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
		
		Mesh mesh;
		Material material;
	};

}