#include "world.hpp"

#include "debugRenderer.hpp"

#include "rayTraversal.hpp"


namespace gfx
{
	void World::update_grid(types::chunk_loc player_loc, bool reload) noexcept
	{

		if (debug.update_world)
		{
			auto v = overworld.manage_chunks(player_loc, reload);

			generate_world(v);

			overworld.add_cmeshes(v);
			overworld.allocate_waiting_cmesh();
		}

		/*== Debug ==*/
		if (debug.show_chunk_borders)
			for (const auto& c : overworld.get_chunkMap())
			{
				if (c.second.isEmpty())
					gfx::aabb((v3f32)(c.second.get_position() + 16ll), { 16.f }, v3f32{ 0, 1, 1 }, 0, false);
				else
					gfx::aabb((v3f32)(c.second.get_position() + 16ll), { 16.f }, v3f32{ 1, 1, 0 }, 0, false);
			}
	}

	void World::generate_world(const std::list<types::chunk_loc>& clocs) noexcept
	{
		for (const auto& i : clocs)
		{
			auto* chunk = overworld.at_chunk(i);
			if (!chunk)
				continue;

			auto c_pos = chunk->get_position();
			bool should_be_empty_chunk{ true };

			for (u16 z{}; z < Chunk::g_size<u16>.z; z++)
			for (u16 x{}; x < Chunk::g_size<u16>.x; x++)
			{
				auto pos = types::voxel_pos{ x, 0, z } + c_pos;

				i64 height = continentalness(terrain_context, pos.z, pos.x) * 10;


				for (u16 y{}; y < Chunk::g_size<u16>.y; y++)
				{
					pos.y = y + c_pos.y;

					if (pos.y <= height)
					{
						should_be_empty_chunk = false;

						chunk->set_voxel_at({ x,y,z }, Voxel{ .type_id{ 1 } });
					}
				}
			}

			if (should_be_empty_chunk)
				chunk->set_empty();
		}
	}

	Voxel World::get_voxel(types::voxel_pos voxel_p) const noexcept
	{
		const auto loc = to_chunkLoc(voxel_p);
		auto* chunk = overworld.at_chunk(loc);

		if (!chunk)
			return {};

		const auto voxel_l = Chunk::to_voxelLoc(*chunk, voxel_p);
		return chunk->at(voxel_l);
	}

	bool World::set_voxel(types::voxel_pos voxel_p, Voxel new_voxel) noexcept
	{
		const auto loc = to_chunkLoc(voxel_p);
		auto* chunk = overworld.at_chunk(loc);

		if (!chunk)
			return false;

		const auto voxel_l = Chunk::to_voxelLoc(*chunk, voxel_p);
		chunk->at(voxel_l) = new_voxel;

		

		if (voxel_l.z == Chunk::g_size<i32>.z - 1)
			overworld.add_priority_cmesh(loc + Chunk::dirs<i64>[4], true);
		if (voxel_l.y == Chunk::g_size<i32>.y - 1)
			overworld.add_priority_cmesh(loc + Chunk::dirs<i64>[2], true);
		if (voxel_l.x == Chunk::g_size<i32>.x - 1)
			overworld.add_priority_cmesh(loc + Chunk::dirs<i64>[0], true);
		if (voxel_l.z == 0)
			overworld.add_priority_cmesh(loc + Chunk::dirs<i64>[5], true);
		if (voxel_l.y == 0)
			overworld.add_priority_cmesh(loc + Chunk::dirs<i64>[3], true);
		if (voxel_l.x == 0)
			overworld.add_priority_cmesh(loc + Chunk::dirs<i64>[1], true);


		overworld.add_priority_cmesh(loc, true);
		

		// Represent each direction index
		const auto check_all_dirs = [&](const size_t i, const size_t first, const size_t second, const size_t i2, const size_t first2, const size_t second2)
			{
				if (voxel_l[i] == Chunk::g_size<i32>[i] - 1 && voxel_l[i2] == Chunk::g_size<i32>[i2] - 1)
					overworld.add_priority_cmesh(loc + Chunk::dirs<i64>[first] + Chunk::dirs<i64>[first2]);
				if (voxel_l[i] == Chunk::g_size<i32>[i] - 1 && voxel_l[i2] == 0)
					overworld.add_priority_cmesh(loc + Chunk::dirs<i64>[first] + Chunk::dirs<i64>[second2]);

				if (voxel_l[i] == 0 && voxel_l[i2] == Chunk::g_size<i32>[i2] - 1)
					overworld.add_priority_cmesh(loc + Chunk::dirs<i64>[second] + Chunk::dirs<i64>[first2]);
				if (voxel_l[i] == 0 && voxel_l[i2] == 0)
					overworld.add_priority_cmesh(loc + Chunk::dirs<i64>[second] + Chunk::dirs<i64>[second2]);
			};

		check_all_dirs(2, 4, 5, 1, 2, 3); 
		check_all_dirs(2, 4, 5, 0, 0, 1); 

		check_all_dirs(1, 2, 3, 0, 0, 1); 
		check_all_dirs(1, 2, 3, 2, 4, 5); 

		check_all_dirs(0, 0, 1, 2, 4, 5); 
		check_all_dirs(0, 0, 1, 1, 2, 3); 


		return true;
	}

	std::optional<RayCastResult> World::raycast(const types::pos& origin, const types::pos& dir, u64 max_length) noexcept
	{
		return gfx::raycast(origin, dir, overworld, max_length);
	}
	

}