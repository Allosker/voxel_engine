#include "gfx_world.hpp"

#include "gfx_debugRenderer.hpp"

namespace gfx
{
	void World::update_grid(types::chunk_loc player_loc, bool reload) noexcept
	{

		if (debug.update_world)
		{
			auto v = overworld.manage_chunks(player_loc, reload);

			generate_world(v);

			overworld.add_cmesh(v);
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
				auto pos = types::voxel_point{ x, 0, z } + c_pos;

				i64 height = continentalness(terrain_context, pos.z, pos.x) * 100;


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

	bool World::set_voxel(types::voxel_point voxel_p, Voxel new_voxel) noexcept
	{
		const auto loc = to_chunkLoc(voxel_p);
		auto* chunk = overworld.at_chunk(loc);

		if (!chunk)
			return false;

		chunk->at(Chunk::to_voxelLoc(*chunk, voxel_p)) = new_voxel;
		overworld.add_cmesh(loc);


		if (loc.z >= Chunk::g_size<i32>.z)
			overworld.add_cmesh(loc + Chunk::dirs<i64>[4]);
		else if (loc.y >= Chunk::g_size<i32>.y)
			overworld.add_cmesh(loc + Chunk::dirs<i64>[2]);
		else if (loc.x >= Chunk::g_size<i32>.x)
			overworld.add_cmesh(loc + Chunk::dirs<i64>[0]);
		else if (loc.z <= 0)
			overworld.add_cmesh(loc + Chunk::dirs<i64>[5]);
		else if (loc.y <= 0)
			overworld.add_cmesh(loc + Chunk::dirs<i64>[3]);
		else if (loc.x <= 0)
			overworld.add_cmesh(loc + Chunk::dirs<i64>[1]);


		return true;
	}
	

}