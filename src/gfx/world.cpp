#include "world.hpp"

#include "debugRenderer.hpp"
#include "meshInstance.hpp"
#include "rayTraversal.hpp"
#include <chrono>


namespace gfx
{
	void World::update_grid(types::chunk_loc player_loc, bool reload) noexcept
	{
		if (debug.update_world)
		{
			if (reload)
				overworld.discard_all_chunks();

			overworld.manage_chunks(player_loc, reload);
			generate_pending_chunks(player_loc);
			overworld.generate_pending_meshes(player_loc, time_budget);
		}

		/*== Debug ==*/
		if (debug.show_chunk_borders)
			for (const auto& c : overworld.get_chunkMap())
			{
				if (c.second.isEmpty())
					gfx::aabb((v3f32)(c.second.get_position() + 16ll), v3f32{ 16.f }, { 0, 1, 1, 1 }, 0, false);
				else
					gfx::aabb((v3f32)(c.second.get_position() + 16ll), v3f32{ 16.f }, { 1, 1, 0, 1 }, 0, false);
			}
	}

	void World::generate_pending_chunks(const types::chunk_loc& player_loc) noexcept
	{
		const auto start = std::chrono::steady_clock::now();

		int generatedCount{};

		auto& chunkGenQueue = overworld.m_chunkGenQueue;
		while(!chunkGenQueue.empty())
		{
			generatedCount++;

			const auto chunkStart = std::chrono::steady_clock::now();

			const auto squareDist = [](const types::chunk_loc& a, const types::chunk_loc& b)
			{
				return (a.x - b.x) * (a.x - b.x) + (a.z - b.z) * (a.z - b.z);
			};
			const auto closest = std::min_element(chunkGenQueue.begin(), chunkGenQueue.end(),
				[&](const types::chunk_loc& a, const types::chunk_loc& b) {
					return squareDist(a, player_loc) < squareDist(b, player_loc);
				}
			);

			const auto elem = *closest;
			chunkGenQueue.erase(closest);

			generate_chunk(elem);

			const auto end = std::chrono::steady_clock::now();

			const auto chunkTime = std::chrono::duration<float>{end - chunkStart}.count();
			const auto totalTime = std::chrono::duration<float>{end - start}.count();

			if (totalTime + chunkTime > time_budget)
			{
				break;
			}
		}

		if (generatedCount)
		{
			const auto end = std::chrono::steady_clock::now();
			const auto totalTime = std::chrono::duration<float>{end - start}.count();
		}
	}

	void World::generate_chunk(const types::chunk_loc& loc) noexcept
	{
		auto* chunk = overworld.at_chunk(loc);
		if (!chunk)
		{
			return;
		}

		const auto c_pos = chunk->get_position();
		bool should_be_empty_chunk{ true };

		generate(terrain_context, terrain_data, (v3f32)chunk->get_position());

		const auto& vtm = VoxelTypeManager::get();
		const auto dirtId = vtm.get_id("dirt"_id);
		const auto stoneId = vtm.get_id("stone"_id);

		for (u16 z{}; z < Chunk::g_size<u16>.z; z++)
		{
			for (u16 x{}; x < Chunk::g_size<u16>.x; x++)
			{
				auto pos = types::voxel_pos{ x, 0, z } + c_pos;

				const i64 height = terrain_data.noise_data.at(x + z * Chunk::g_size<size_t>.z) * 10;
				const i64 localHeight = height - c_pos.y;

				if (localHeight < 0)
				{
					continue;
				}

				should_be_empty_chunk = false;

				if (localHeight < Chunk::g_size<size_t>.y)
				{
					chunk->set_voxel_at({ x,localHeight,z }, Voxel{ dirtId });
				}

				for (u16 y = 0; y < std::min(localHeight, Chunk::g_size<i64>.x); y++)
				{
					chunk->set_voxel_at({ x,y,z }, Voxel{ vtm.get_id("stone") });
				}
			}
		}

		if (should_be_empty_chunk)
		{
			chunk->set_empty();
		}
		else
		{
			overworld.dirty_cmesh(loc);
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
		auto& c_voxel = chunk->at(voxel_l);

		if (new_voxel.type_id == types::type_id_null)
			add_entity(loc, WorldItem{ c_voxel.type_id, static_cast<v3f64>(voxel_p) });

		c_voxel = new_voxel;


		if (voxel_l.z == Chunk::g_size<i32>.z - 1)
			overworld.dirty_cmesh(loc + Chunk::dirs<i64>[4]);
		if (voxel_l.y == Chunk::g_size<i32>.y - 1)
			overworld.dirty_cmesh(loc + Chunk::dirs<i64>[2]);
		if (voxel_l.x == Chunk::g_size<i32>.x - 1)
			overworld.dirty_cmesh(loc + Chunk::dirs<i64>[0]);
		if (voxel_l.z == 0)
			overworld.dirty_cmesh(loc + Chunk::dirs<i64>[5]);
		if (voxel_l.y == 0)
			overworld.dirty_cmesh(loc + Chunk::dirs<i64>[3]);
		if (voxel_l.x == 0)
			overworld.dirty_cmesh(loc + Chunk::dirs<i64>[1]);


		overworld.dirty_cmesh(loc);

		
		// Represent each direction index
		const auto check_all_dirs = [&](const size_t i, const size_t first, const size_t second, const size_t i2, const size_t first2, const size_t second2)
			{
				if (voxel_l[i] == Chunk::g_size<i32>[i] - 1 && voxel_l[i2] == Chunk::g_size<i32>[i2] - 1)
					overworld.dirty_cmesh(loc + Chunk::dirs<i64>[first] + Chunk::dirs<i64>[first2]);
				if (voxel_l[i] == Chunk::g_size<i32>[i] - 1 && voxel_l[i2] == 0)
					overworld.dirty_cmesh(loc + Chunk::dirs<i64>[first] + Chunk::dirs<i64>[second2]);

				if (voxel_l[i] == 0 && voxel_l[i2] == Chunk::g_size<i32>[i2] - 1)
					overworld.dirty_cmesh(loc + Chunk::dirs<i64>[second] + Chunk::dirs<i64>[first2]);
				if (voxel_l[i] == 0 && voxel_l[i2] == 0)
					overworld.dirty_cmesh(loc + Chunk::dirs<i64>[second] + Chunk::dirs<i64>[second2]);
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


	void World::draw(Renderer& renderer)
	{
		get_chunkGrid().draw(renderer);


		for (i64 z{ get_chunkGrid().get_min().z}; z <= get_chunkGrid().get_max().z; z++)
		{
			for (i64 y{ get_chunkGrid().get_min().y }; y <= get_chunkGrid().get_max().y; y++)
			{
				for (i64 x{ get_chunkGrid().get_min().x }; x <= get_chunkGrid().get_max().x; x++)
				{
					const auto loc = types::chunk_loc{ x,y,z };

					auto it = m_entities.find(loc);
					if (it != m_entities.end())
						for (auto& entity : it->second)
						{
							entity.draw(renderer);
						}
				}
			}
		}

		
		for (auto& meshInstance : m_meshInstances)
		{
			meshInstance.draw(renderer);
		}
	}
}