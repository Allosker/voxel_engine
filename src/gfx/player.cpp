#include "player.hpp"

#include <vector>

#include "chunk.hpp"
#include "voxelType.hpp"
#include <array>
#include <optional>

void gfx::Player::move(Keys key, f64 dt) noexcept
{
	switch (key)
	{
		case Keys::W:
			m_mov.velocity += glm::normalize(v3f64{ m_cam->get_front().x, 0., m_cam->get_front().z }) * m_mov.acceleration * dt;
			m_mov.moving_hor = true;
			break;

		case Keys::S:
			m_mov.velocity -= glm::normalize(v3f64{ m_cam->get_front().x, 0., m_cam->get_front().z }) * m_mov.acceleration * dt;
			m_mov.moving_hor = true;
			break;

		case Keys::A:
			m_mov.velocity -= glm::normalize(v3f64{ m_cam->get_right().x, 0., m_cam->get_right().z }) * m_mov.acceleration * dt;
			m_mov.moving_hor = true;
			break;

		case Keys::D:
			m_mov.velocity += glm::normalize(v3f64{ m_cam->get_right().x, 0., m_cam->get_right().z }) * m_mov.acceleration * dt;
			m_mov.moving_hor = true;
			break;


		case Keys::Space:
			if (flying)
			{
				if (m_mov.velocity.y < 0.)
					m_mov.velocity.y = 0.;

				m_mov.velocity.y += m_cam->get_up().y * m_mov.acceleration * dt;
				m_mov.moving_ver = true;
			}
			else if (is_on_ground)
			{
				m_mov.velocity.y += m_cam->get_up().y * jump_velocity;
			}
			break;

		case Keys::Left_shift:
			if (flying)
			{
				if (m_mov.velocity.y > 0.)
					m_mov.velocity.y = 0.;

				m_mov.velocity -= v3f64{ 0, m_cam->get_up().y, 0. } * m_mov.acceleration * dt;
				m_mov.moving_ver = true;
			}
			break;
	}
}

void gfx::Player::update_position(World& world, f64 dt) noexcept
{
	m_mov.velocity = phy::calculate_velocity(m_mov, dt, world.gravity, !flying && !m_mov.moving_ver);

	set_pos(m_trans.get_pos() + m_mov.velocity * dt);

	m_mov.moving_hor = false;
	m_mov.moving_ver = false;
}

void gfx::Player::resolve_collisions_entities(World& world, PlayerInventory& inv, f64 dt) noexcept
{
	auto hitbox = m_hitbox;
	hitbox.move(get_pos());

	std::optional<types::chunk_loc> temp{ std::nullopt };
	for (const auto& i : phy::get_corners(hitbox))
	{
		const auto& loc = World::to_chunkLoc(World::to_voxelPos(i));

		if (!temp || loc != *temp)
		{
			world.remove_entities_if(loc, [&](const WorldItem& item)
			{
				if (phy::intersects(hitbox, item.get_hitbox()))
				{
					inv.get_inventory().add_items({ item.get_id(), {} }, 1);
					return true;
				}
				return false;
			});

			temp = std::make_optional(loc);
		}
	}

	

}

void gfx::Player::resolve_collisions_world(World& world, f64 dt) noexcept
{
	is_on_ground = false;

	if (ghost)
		return;

	std::vector<types::voxel_pos> voxel_positions;

	auto hitbox = m_hitbox;
	hitbox.move(get_pos());


	const Chunk* chunk = nullptr;

	bool correction_already_applied_y{};

	const auto floored_pos_min = World::to_voxelPos(hitbox.get_min());
	const auto floored_pos_max = World::to_voxelPos(hitbox.get_max());
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



					if (phy::intersects(hitbox, voxel))
					{
						auto offset = phy::get_MTV(hitbox, voxel);

						if (offset.x == 0. && offset.y == 0. && offset.z == 0.) continue;


						if (VoxelTypeManager::get().get_type(world.get_voxel(World::to_voxelPos(static_cast<types::pos>(pos) - glm::normalize(offset)))).has_bounds)
							offset = {};

						if (offset.y != 0)
							m_mov.velocity.y = 0;

						if (offset.x != 0)
							m_mov.velocity.x = 0;

						if (offset.z != 0)
							m_mov.velocity.z = 0;

						if (offset.y <= 0)
							is_on_ground = true;

						/*r
						* This fixes the jerky motions since it occurred when multiple voxels tried to resolve collisions on the y axis
						* which caused the player to be over-offset-ed.
						*/
						if (correction_already_applied_y)
						{
							offset.y = 0;
						}
						else if (!correction_already_applied_y && offset.y != 0)
							correction_already_applied_y = true;

						set_pos(get_pos() - offset);
						
					}
				}
			}
}
