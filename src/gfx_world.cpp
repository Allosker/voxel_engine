#include "gfx_world.hpp"

#include "gfx_debugRenderer.hpp"

namespace gfx
{
	void World::update_grid(types::chunk_loc player_loc, bool reload) noexcept
	{
		auto v = overworld.manage_chunks(player_loc, reload);

		generate_world(v);
		
		overworld.add_cmesh(v);
		overworld.allocate_waiting_cmesh();

		/*== Debug ==*/
		if (debug.show_chunk_borders)
			for (const auto& c : overworld.get_chunkMap())
			{
				gfx::aabb((v3f32)(c.second.get_position() + 16ll), { 16.f }, (c.second.isEmpty() ? v3f32{ 0, 1, 1 } : v3f32{ 1, 1, 0 }), 0, false);
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
	

}