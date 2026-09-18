#include "chunkMesh.hpp"

#include <cassert>

#include "voxelType.hpp"
#include "chunkGrid.hpp"
#include "sys/assetsManager.hpp"

namespace gfx
{
namespace
{
	std::array<f32, 4> calculate_ao(auto index, const auto& v_loc_dir, auto& atSolidGrid) noexcept
	{
		std::array<f32, 4> physical_ao{ 1.f, 1.f, 1.f, 1.f };

		// First, fetch whether each block around the face is empty or not
		// This is stored as a bool since we only care about whether it is transparent or not
		std::array<bool, 8> ao_full_dirs{};
		for (size_t j{}; j < ao_full_dirs.size(); j++)
		{
			auto v_check = v_loc_dir + ChunkMesh::ao_dirs<i32>[index][j];
			ao_full_dirs[j] = !atSolidGrid(v_check);
		}

		// Set up the AO value according to the blocks surrounding the current face
		constexpr size_t si{ ChunkMesh::ao_values.size() - 1 };

		const bool any_v_full =
			ao_full_dirs[0] || ao_full_dirs[1] || ao_full_dirs[2] ||
			ao_full_dirs[3] || ao_full_dirs[4] || ao_full_dirs[5] ||
			ao_full_dirs[6] || ao_full_dirs[7];


		if (ao_full_dirs[6] && ao_full_dirs[4])
			physical_ao[0] = ChunkMesh::ao_values[0];
		else if (any_v_full)
			physical_ao[0] = ChunkMesh::ao_values[si - (ao_full_dirs[4] + ao_full_dirs[6] + ao_full_dirs[5])];

		if (ao_full_dirs[4] && ao_full_dirs[2])
			physical_ao[1] = ChunkMesh::ao_values[0];
		else if (any_v_full)
			physical_ao[1] = ChunkMesh::ao_values[si - (ao_full_dirs[4] + ao_full_dirs[2] + ao_full_dirs[3])];

		if (ao_full_dirs[6] && ao_full_dirs[0])
			physical_ao[2] = ChunkMesh::ao_values[0];
		else if (any_v_full)
			physical_ao[2] = ChunkMesh::ao_values[si - (ao_full_dirs[6] + ao_full_dirs[0] + ao_full_dirs[7])];

		if (ao_full_dirs[0] && ao_full_dirs[2])
			physical_ao[3] = ChunkMesh::ao_values[0];
		else if (any_v_full)
			physical_ao[3] = ChunkMesh::ao_values[si - (ao_full_dirs[0] + ao_full_dirs[2] + ao_full_dirs[1])];


		switch (index)
		{
		case 0: // Left (+x)
			return {
				physical_ao[1],
				physical_ao[0],
				physical_ao[3],
				physical_ao[2]
			};

		case 1: // Right (-x)
			return {
				physical_ao[3],
				physical_ao[2],
				physical_ao[1],
				physical_ao[0]
			};

		case 2: // Up (+y)
			return {
				physical_ao[3],
				physical_ao[2],
				physical_ao[1],
				physical_ao[0]
			};

		case 3: // Down (-y)
			return {
				physical_ao[1],
				physical_ao[0],
				physical_ao[3],
				physical_ao[2]
			};

		case 4: // Front (+z)
			return {
				physical_ao[0],
				physical_ao[2],
				physical_ao[1],
				physical_ao[3]
			};

		case 5: // Back (-z)
			return {
				physical_ao[1],
				physical_ao[3],
				physical_ao[0],
				physical_ao[2]
			};

		default:
			assert("ERROR::CHUNKMESH::AO_CALCULATING::Cannot calculate AO for the given face, index out of bound");
			return {};
		}
	}
}

	static void assemble_pos_uvs(auto& out, const auto& pos, const std::vector<v2f32>& uvs, const std::array<f32, 4>& ao, const v3f32& xyz) noexcept
	{
		out.emplace_back(ChunkMesh::VoxelVertex{ pos[0] + xyz, uvs[0], ao[0] });
		out.emplace_back(ChunkMesh::VoxelVertex{ pos[1] + xyz, uvs[1], ao[1] });
		out.emplace_back(ChunkMesh::VoxelVertex{ pos[2] + xyz, uvs[2], ao[2] });
		out.emplace_back(ChunkMesh::VoxelVertex{ pos[3] + xyz, uvs[3], ao[1] });
		out.emplace_back(ChunkMesh::VoxelVertex{ pos[4] + xyz, uvs[4], ao[3] });
		out.emplace_back(ChunkMesh::VoxelVertex{ pos[5] + xyz, uvs[5], ao[2] });
	}

	ChunkMesh::ChunkMesh(const Chunk& current_chunk, const ChunkGrid& grid) noexcept
		: material(&AssetsManager::get().shaders.at("shaders/world_chunks"))
	{
		mesh.create_buffer<VoxelVertex>(false);
		update_mesh(bake_mesh(current_chunk, grid));
		
		material.set("tex", &AssetsManager::get().textures.at("textures/voxels/atlas"));
	
		move(current_chunk.get_position());
	}

	std::vector<ChunkMesh::VoxelVertex> ChunkMesh::bake_mesh(const Chunk& current_chunk, const ChunkGrid& grid) noexcept
	{
		std::vector<VoxelVertex> ret;

		const auto edgedGridSize = Chunk::g_size<i32>.z + 2;
		std::vector<Voxel> edgedGrid(edgedGridSize * edgedGridSize * edgedGridSize);

		const auto atEdgeGrid = [&](auto pos) -> auto&
		{
			return edgedGrid[(pos.x + 1) + (pos.y + 1) * edgedGridSize + (pos.z + 1) * edgedGridSize * edgedGridSize];
		};

		const Voxel* chunkPtr = &current_chunk.at({});
		Voxel* edgePtr = edgedGrid.data();

		edgePtr += edgedGridSize * edgedGridSize;

		for (i32 z{ 0 }; z < Chunk::g_size<i32>.z; z++)
		{
			edgePtr += edgedGridSize;

			for (i32 y{ 0 }; y < Chunk::g_size<i32>.y; y++)
			{
				edgePtr++;

				std::memcpy(edgePtr, chunkPtr, Chunk::g_size<i32>.x * sizeof(Voxel));

				edgePtr += Chunk::g_size<i32>.x;
				chunkPtr += Chunk::g_size<i32>.x;

				edgePtr++;
			}

			edgePtr += edgedGridSize;
		}

		const auto cloc = current_chunk.get_location();

		for (i32 x : {-1, Chunk::g_size<i32>.z})
		{
			auto* chunk = x < 0 ? grid.at_chunk(cloc + types::chunk_loc{ -1, 0, 0 }) : grid.at_chunk(cloc + types::chunk_loc{ 1, 0, 0 });
			if (!chunk || chunk->isEmpty())
			{
				continue;
			}

			const Voxel* chunkPtr = &chunk->at({});
			Voxel* edgePtr = edgedGrid.data();

			edgePtr += edgedGridSize * edgedGridSize;

			if (x < 0)
			{
				chunkPtr += Chunk::g_size<i32>.y - 1;
			}
			else
			{
				edgePtr += edgedGridSize - 1;
			}
			
			for (i32 z{ 0 }; z < Chunk::g_size<i32>.z; z++)
			{
				edgePtr += edgedGridSize;

				for (i32 y{ 0 }; y < Chunk::g_size<i32>.y; y++)
				{
					*edgePtr = *chunkPtr;

					chunkPtr += Chunk::g_size<i32>.y;

					edgePtr	+= edgedGridSize;
;
				}

				edgePtr += edgedGridSize;
			}
		}

		for (i32 y : {-1, Chunk::g_size<i32>.z})
		{
			auto* chunk = y < 0 ? grid.at_chunk(cloc + types::chunk_loc{ 0, -1, 0 }) : grid.at_chunk(cloc + types::chunk_loc{ 0, 1, 0 });
			if (!chunk || chunk->isEmpty())
			{
				continue;
			}
			const auto localY = y < 0 ? Chunk::g_size<i32>.z - 1 : 0;
			for (i32 z = 0; z < Chunk::g_size<i32>.z; z++)
			{
				for (i32 x = 0; x < Chunk::g_size<i32>.x; x++)
				{
					types::voxel_pos v_loc{ x, y, z };
					atEdgeGrid(v_loc) = chunk->at({ x, localY, z });
				}
			}
		}

		for (i32 z : {-1, Chunk::g_size<i32>.z})
		{
			auto* chunk = z < 0 ? grid.at_chunk(cloc + types::chunk_loc{ 0, 0, -1 }) : grid.at_chunk(cloc + types::chunk_loc{ 0, 0, 1 });
			if (!chunk || chunk->isEmpty())
			{
				continue;
			}
			const auto localZ = z < 0 ? Chunk::g_size<i32>.z - 1 : 0;
			for (i32 y = 0; y < Chunk::g_size<i32>.y; y++)
			{
				for (i32 x = 0; x < Chunk::g_size<i32>.x; x++)
				{
					types::voxel_pos v_loc{ x, y, z };
					atEdgeGrid(v_loc) = chunk->at({ x, y, localZ });
				}
			}
		}

		std::vector<uint8_t> solidGrid(edgedGridSize * edgedGridSize * edgedGridSize);
		for (int x = 0; x < solidGrid.size(); x++)
		{
			solidGrid[x] = !VoxelTypeManager::get().get_type(edgedGrid[x].type_id).is_transparent;
		}

		const auto atSolidGrid = [&](auto pos) -> auto
		{
			return solidGrid[(pos.x + 1) + (pos.y + 1) * edgedGridSize + (pos.z + 1) * edgedGridSize * edgedGridSize];
		};

		const auto start = std::chrono::steady_clock::now();
		for (i32 z{}; z < Chunk::g_size<i32>.z; z++)
		{
			for (i32 y{}; y < Chunk::g_size<i32>.y; y++)
			{
				for (i32 x{}; x < Chunk::g_size<i32>.x; x++)
				{
					v3i32 v_loc{ x, y, z };
					if (!atSolidGrid(v_loc))
						continue;

					const auto voxPos = v3f32{ (f32)x,(f32)y,(f32)z };

					for (size_t i{}; i < Chunk::dirs<i8>.size(); i++)
					{
						const auto v_loc_dir{ v_loc + Chunk::dirs<i32>[i] };
						const bool is_face_visible = !atSolidGrid(v_loc_dir);

						if (is_face_visible)
						{
							assemble_pos_uvs(
								ret,
								Voxel::g_model[i],
								calculate_uvs(atEdgeGrid(v_loc).type_id),
								calculate_ao(i, v_loc_dir, atSolidGrid),
								voxPos
							);
						}
					}
				}
			}
		}

		const auto end = std::chrono::steady_clock::now();
		const auto totalTime = std::chrono::duration<float>{ end - start }.count();

		std::println("mesh gen: {}ms", totalTime * 1000.f);

		return ret;
	}

	void ChunkMesh::update_mesh(const std::vector<VoxelVertex>& vertices) noexcept
	{
		mesh.update_buffer(vertices);
	}
}