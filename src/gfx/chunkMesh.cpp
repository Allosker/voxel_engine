#include "chunkMesh.hpp"

#include <cassert>

#include "voxelType.hpp"
#include "chunkGrid.hpp"

namespace gfx
{

	static void assemble_pos_uvs(auto& out, const auto& pos, const auto& uvs, const std::array<f32, 4>& ao, const v3f32& xyz) noexcept
	{
		out.emplace_back(ChunkMesh::VoxelVertex{ pos[0] + xyz, uvs[0], ao[0] });
		out.emplace_back(ChunkMesh::VoxelVertex{ pos[1] + xyz, uvs[1], ao[1] });
		out.emplace_back(ChunkMesh::VoxelVertex{ pos[2] + xyz, uvs[2], ao[2] });
		out.emplace_back(ChunkMesh::VoxelVertex{ pos[3] + xyz, uvs[3], ao[1] });
		out.emplace_back(ChunkMesh::VoxelVertex{ pos[4] + xyz, uvs[4], ao[3] });
		out.emplace_back(ChunkMesh::VoxelVertex{ pos[5] + xyz, uvs[5], ao[2] });
	}

	/// <summary>
	/// Calculate the AO components for one face as an array representing each corner (readjusted depending on the direction)
	/// </summary>
	/// <param name="The chunk from which the face comes"></param>
	/// <param name="Current Voxel Index"></param>
	/// <param name="Location of the block in front of the face"></param>
	/// <param name="The chunk grid from which the target chunk comes"></param>
	/// <returns>The ao value for each corner</returns>
	static std::array<f32, 4> calculate_ao(const auto* targetChunk, auto index, const auto& v_loc_dir, const ChunkGrid& grid) noexcept
	{
		std::array<f32, 4> physical_ao{ 1.f, 1.f, 1.f, 1.f };

		if (!targetChunk) // If current chunk doesn't exist, we needn't check anything
			return physical_ao;

		// First, fetch whether each block around the face is empty or not
		// This is stored as a bool since we only care about whether it is transparent or not
		std::array<bool, 8> ao_full_dirs{};
		for (size_t j{}; j < ao_full_dirs.size(); j++)
		{
			auto v_check = v_loc_dir + ChunkMesh::ao_dirs<i32>[index][j];

			types::chunk_loc final_loc{ targetChunk->get_location() };
			if (v_check.z >= Chunk::g_size<i32>.z)
			{
				v_check.z = 0;
				final_loc += Chunk::dirs<i64>[4];
			}
			if (v_check.y >= Chunk::g_size<i32>.y)
			{
				v_check.y = 0;
				final_loc += Chunk::dirs<i64>[2];
			}
			if (v_check.x >= Chunk::g_size<i32>.x)
			{
				v_check.x = 0;
				final_loc += Chunk::dirs<i64>[0];
			}
			if (v_check.z < 0)
			{
				v_check.z = Chunk::g_size<i32>.z - 1;
				final_loc += Chunk::dirs<i64>[5];
			}
			if (v_check.y < 0)
			{
				v_check.y = Chunk::g_size<i32>.y - 1;
				final_loc += Chunk::dirs<i64>[3];
			}
			if (v_check.x < 0)
			{
				v_check.x = Chunk::g_size<i32>.x - 1;
				final_loc += Chunk::dirs<i64>[1];
			}

			if (const auto* ao_targetChunk = grid.at_chunk(final_loc))
				ao_full_dirs[j] = !VoxelTypeManager::get().get_type(ao_targetChunk->at(static_cast<v3u16>(v_check)).type_id).is_transparent;
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




	ChunkMesh::ChunkMesh(const Chunk& current_chunk, const ChunkGrid& grid) noexcept
	{
		create_buffers();

		update_mesh(bake_mesh(current_chunk, grid));
	}

	ChunkMesh::ChunkMesh(ChunkMesh&& other) noexcept
		: m_vao{ other.m_vao }, m_vbo{ other.m_vbo }, m_vertices_count{ other.m_vertices_count }
	{
		other.m_vao = 0;
		other.m_vbo = 0;
		other.m_vertices_count = 0;
	}

	ChunkMesh& ChunkMesh::operator=(ChunkMesh&& other) noexcept
	{
		if (this == &other)
			return *this;

		m_vao = other.m_vao = 0;
		m_vbo = other.m_vbo = 0;
		m_vertices_count = other.m_vertices_count = 0;

		other.m_vao = 0;
		other.m_vbo = 0;
		other.m_vertices_count = 0;

		return *this;
	}


	std::vector<ChunkMesh::VoxelVertex> ChunkMesh::bake_mesh(const Chunk& current_chunk, const ChunkGrid& grid) noexcept
	{
		std::vector<VoxelVertex> ret;

		const std::array<const Chunk* const, 6> chunk_dirs
		{
			grid.at_chunk(current_chunk.get_location() + Chunk::dirs<i64>[0]),
			grid.at_chunk(current_chunk.get_location() + Chunk::dirs<i64>[1]),
			grid.at_chunk(current_chunk.get_location() + Chunk::dirs<i64>[2]),
			grid.at_chunk(current_chunk.get_location() + Chunk::dirs<i64>[3]),
			grid.at_chunk(current_chunk.get_location() + Chunk::dirs<i64>[4]),
			grid.at_chunk(current_chunk.get_location() + Chunk::dirs<i64>[5])
		};


		for (i32 z{}; z < Chunk::g_size<i32>.z; z++)
		{
			for (i32 y{}; y < Chunk::g_size<i32>.y; y++)
			{
				for (i32 x{}; x < Chunk::g_size<i32>.x; x++)
				{
					v3i32 v_loc{ x, y, z };
					const auto& voxel = current_chunk.at(static_cast<v3u16>(v_loc));

					if (voxel.type_id == 0)
						continue;

					std::array<f32, 4> ao_corners{};

					for (size_t i{}; i < Chunk::dirs<i8>.size(); i++)
					{
						bool is_face_visible{ false };
						auto v_loc_dir{ v_loc + Chunk::dirs<i32>[i] };
						const auto* targetChunk = &current_chunk;


						if (v_loc_dir.z >= Chunk::g_size<i32>.z)
						{
							v_loc_dir.z = 0;
							targetChunk = chunk_dirs[i];
						}
						else if (v_loc_dir.y >= Chunk::g_size<i32>.y)
						{
							v_loc_dir.y = 0;
							targetChunk = chunk_dirs[i];
						}
						else if (v_loc_dir.x >= Chunk::g_size<i32>.x)
						{
							v_loc_dir.x = 0;
							targetChunk = chunk_dirs[i];
						}
						else if (v_loc_dir.z < 0)
						{
							v_loc_dir.z = Chunk::g_size<i32>.z - 1;
							targetChunk = chunk_dirs[i];
						}
						else if (v_loc_dir.y < 0)
						{
							v_loc_dir.y = Chunk::g_size<i32>.y - 1;
							targetChunk = chunk_dirs[i];
						}
						else if (v_loc_dir.x < 0)
						{
							v_loc_dir.x = Chunk::g_size<i32>.x - 1;
							targetChunk = chunk_dirs[i];
						}

						if (targetChunk && VoxelTypeManager::get().get_type(targetChunk->at(static_cast<v3u16>(v_loc_dir)).type_id).is_transparent)
							is_face_visible = true;


						if (is_face_visible)
						{
							assemble_pos_uvs(
								ret,
								Voxel::model[i],
								Voxel::face_uvs, 
							calculate_ao(targetChunk, i, v_loc_dir, grid),
							v3f32{ (f32)x,(f32)y,(f32)z } + static_cast<v3f32>(current_chunk.get_position())
							);
						}

					}

				}
			}
		}

		return ret;
	}

	void ChunkMesh::update_mesh(const std::vector<VoxelVertex>& vertices) noexcept
	{
		if (!m_vao || !m_vbo)
			create_buffers();

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);


		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(vertices.size()) * sizeof(VoxelVertex), vertices.data(), GL_STATIC_DRAW);
		m_vertices_count = static_cast<GLsizei>(vertices.size());


		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void ChunkMesh::create_buffers() noexcept
	{
		glCreateVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);


		glVertexAttribPointer(0, v3f32::length(), GL_FLOAT, false, sizeof(VoxelVertex), std::bit_cast<void*>(offsetof(VoxelVertex, position)));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, v2f32::length(), GL_FLOAT, false, sizeof(VoxelVertex), std::bit_cast<void*>(offsetof(VoxelVertex, uvs)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 1/*ao value*/, GL_FLOAT, false, sizeof(VoxelVertex), std::bit_cast<void*>(offsetof(VoxelVertex, ao)));
		glEnableVertexAttribArray(2);


		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}


	void ChunkMesh::free_resources() noexcept
	{
		glDeleteBuffers(1, &m_vbo);
		glDeleteVertexArrays(1, &m_vao);
		m_vertices_count = 0;
	}


}