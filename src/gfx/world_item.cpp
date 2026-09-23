#include "world_item.hpp"

#include "player.hpp"
#include "gfx/world.hpp"


namespace gfx
{

	void WorldItem::update(const World& world, f64 dt) noexcept
	{
		m_mov.velocity = phy::calculate_velocity(m_mov, dt, world.gravity, true);

		move(m_mov.velocity * dt);

		m_mov.moving_hor = false;
		m_mov.moving_ver = false;


		const Chunk* chunk = nullptr;

		const auto& floored_pos_min = World::to_voxelPos(m_hitbox.get_min());
		const auto& floored_pos_max = World::to_voxelPos(m_hitbox.get_max());

		for (i64 x{ floored_pos_min.x }; x <= floored_pos_max.x; x++)
		for (i64 y{ floored_pos_min.y }; y <= floored_pos_max.y; y++)
		for (i64 z{ floored_pos_min.z }; z <= floored_pos_max.z; z++)
		{
			const types::voxel_pos pos{ x, y, z };

			const auto chunk_loc = World::to_chunkLoc(pos);
			chunk = world.get_chunkGrid().at_chunk(chunk_loc);

			if (!chunk)
				continue;


			if (const auto* vptr{ chunk->at_ptr(Chunk::to_voxelLoc(*chunk, pos)) };
				vptr && VoxelTypeManager::get().get_type(vptr->type_id).has_bounds)
			{
				phy::HitboxAABB voxel{ static_cast<v3f64>(pos) + 0.5, v3f64{ 0.5 } };

				if (phy::intersects(m_hitbox, voxel))
				{
					auto offset = phy::get_MTV(m_hitbox, voxel);

					if (offset.x == 0. && offset.y == 0. && offset.z == 0.) continue;


					if (VoxelTypeManager::get().get_type(world.get_voxel(World::to_voxelPos(static_cast<types::pos>(pos) - glm::normalize(offset)))).has_bounds)
						offset = {};

					if (offset.y != 0)
						m_mov.velocity.y = 0;

					if (offset.x != 0)
						m_mov.velocity.x = 0;

					if (offset.z != 0)
						m_mov.velocity.z = 0;

					if (offset.y < 0)
						is_on_ground = true;

					move(-offset);
				}
			}
		}

	}


}