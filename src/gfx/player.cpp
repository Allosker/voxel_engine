#include "player.hpp"

#include <vector>

#include "chunk.hpp"
#include "voxelType.hpp"


void gfx::Player::move(Keys key, f64 dt) noexcept
{
	switch (key)
	{
		case Keys::W:
			m_mov.velocity += v3f64{ m_cam->get_front().x, 0., m_cam->get_front().z }.normal() * m_mov.speed * dt;
			m_mov.moving_hor = true;
			break;

		case Keys::S:
			m_mov.velocity -= v3f64{ m_cam->get_front().x, 0., m_cam->get_front().z }.normal() * m_mov.speed * dt;
			m_mov.moving_hor = true;
			break;

		case Keys::A:
			m_mov.velocity -= v3f64{ m_cam->get_right().x, 0., m_cam->get_right().z }.normal() * m_mov.speed * dt;
			m_mov.moving_hor = true;
			break;

		case Keys::D:
			m_mov.velocity += v3f64{ m_cam->get_right().x, 0., m_cam->get_right().z }.normal() * m_mov.speed * dt;
			m_mov.moving_hor = true;
			break;


		case Keys::Space:
			if (m_mov.flying)
			{
				if (m_mov.velocity.y < 0.)
					m_mov.velocity.y = 0.;
				m_mov.velocity.y += m_mov.jump_height * dt;
				m_mov.moving_ver = true;
			}
			else if (!m_mov.jumped_this_frame)
			{
				m_mov.velocity += v3f64{ 0,  m_mov.jump_height, 0. };
				m_mov.moving_ver = true;

				m_mov.jumped_this_frame = true;
			}
			break;

		case Keys::Left_shift:
			if (m_mov.flying)
			{
				if (m_mov.velocity.y > 0.)
					m_mov.velocity.y = 0.;
				m_mov.velocity.y -= m_cam->get_up().y * m_mov.jump_height * dt;
				m_mov.moving_ver = true;
			}
			break;
	}
}

void gfx::Player::update_position(const World& world, f64 dt) noexcept
{

	if (v2f64{ m_mov.velocity.x, m_mov.velocity.z }.length_squared() > m_mov.max_speed * m_mov.max_speed)
	{
		const auto tempY{ m_mov.velocity.y };

		m_mov.velocity = v3f64{ m_mov.velocity.x, 0, m_mov.velocity.z }.normal() * m_mov.max_speed;
		m_mov.velocity.y = tempY;
	}


	if (!m_mov.moving_hor)
	{
		m_mov.velocity.x *= (1 - m_mov.friction * dt);
		m_mov.velocity.z *= (1 - m_mov.friction * dt);
	}

	if (!m_mov.flying && !m_mov.moving_ver && m_mov.velocity.y != m_mov.y_padding)
		m_mov.velocity.y += world.gravity * dt;
	else if (!m_mov.moving_ver)
		m_mov.velocity.y = 0.;



	set_pos(m_trans.get_pos() + m_mov.velocity * dt);


	m_mov.moving_hor = false;
	m_mov.moving_ver = false;
}

void gfx::Player::resolve_collisions(const World& world, f64 dt) noexcept
{
	if (m_mov.ghost)
		return;

	std::vector<types::voxel_pos> voxel_positions;

	auto hitbox = m_hitbox;
	hitbox.move(get_pos());

	if (debug.show_hitbox)
		aabb_min_max((v3f32)hitbox.get_min(), (v3f32)hitbox.get_max(), { 1, 0, 0 }, 0., false);

	const auto flooredPosMin{ World::to_voxelPos(hitbox.get_min()) };
	const auto flooredPosMax{ World::to_voxelPos(hitbox.get_max()) };



	const Chunk* chunk = nullptr;
	const Chunk* outter_chunk = nullptr;

	for (i64 x{ flooredPosMin.x }; x <= flooredPosMax.x + 1; x++)
	for (i64 y{ flooredPosMin.y }; y <= flooredPosMax.y + 1; y++)
	for (i64 z{ flooredPosMin.z }; z <= flooredPosMax.z + 1; z++)
	{
		const types::voxel_pos pos{ x, y, z };

		const auto chunk_loc = World::to_chunkLoc(pos);
		chunk = world.get_chunkGrid().at_chunk(chunk_loc);

		if (!chunk)
			continue;


		if (const auto* vptr{ chunk->at_ptr(Chunk::to_voxelLoc(*chunk, pos)) };
			vptr && VoxelTypeManager::get().get_type(vptr->type_id).has_bounds)
		{
			phy::HitboxAABB voxel{ 0.5, { 0.5 } };
			voxel.move(static_cast<types::pos>(pos));

			aabb_min_max((v3f32)voxel.get_min(), (v3f32)voxel.get_max(), { 1, 1, 1 }, 0., false);


			if (phy::intersects(hitbox, voxel))
			{
				auto offset = phy::get_MTV(hitbox, voxel);

				if (offset.x == 0. && offset.y == 0. && offset.z == 0.) continue;


				if (VoxelTypeManager::get().get_type(world.get_voxel(World::to_voxelPos(static_cast<types::pos>(pos) - offset.normal())).type_id).has_bounds)
					offset = {};

				if (offset.y != 0)
					m_mov.velocity.y = 0 + m_mov.y_padding;

				if (offset.x != 0)
					m_mov.velocity.x = 0;

				if (offset.z != 0)
					m_mov.velocity.z = 0;


				set_pos(get_pos() - offset);
			}
		}
	}
}
