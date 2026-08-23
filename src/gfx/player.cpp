#include "player.hpp"

#include <vector>

#include "chunk.hpp"
#include "voxelTypeManager.hpp"


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
			m_mov.velocity += v3f64{ 0, m_cam->get_up().y, 0. } * m_mov.jump_height * dt;
			m_mov.moving_ver = true;
			break;

		case Keys::Left_shift:
			m_mov.velocity -= v3f64{ 0, m_cam->get_up().y, 0. } * m_mov.jump_height * dt;
			m_mov.moving_ver = true;
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

	if (!m_mov.flying && !m_mov.moving_ver)
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

	const auto flooredPosMin{ static_cast<v3i64>(mpml::floor(m_hitbox.get_min())) };
	const auto flooredPosMax{ static_cast<v3i64>(mpml::floor(m_hitbox.get_max())) };

	for (i64 x{ flooredPosMin.x }; x <= flooredPosMax.x; x++)
		for (i64 y{ flooredPosMin.y }; y <= flooredPosMax.y; y++)
			for (i64 z{ flooredPosMin.z }; z <= flooredPosMax.z; z++)
				voxel_positions.emplace_back(types::voxel_pos{ x, y, z });



	const Chunk* chunk = nullptr;

	for (const auto& pos : voxel_positions)
	{
		auto chunk_loc = World::to_chunkLoc(pos);
		chunk = world.get_chunkGrid().at_chunk(chunk_loc);

		if (!chunk)
			continue;

		if (const auto* vptr{ chunk->at_ptr(Chunk::to_voxelLoc(*chunk, pos)) };
			vptr && VoxelTypeManager::get().get_type(vptr->type_id).has_bounds)
		{
			phy::HitboxAABB voxel{ static_cast<v3f64>(pos), {1.} };
			aabb((v3f32)pos, { 1. }, { 1, 1, 1 }, 0., false);


			if (m_hitbox.intersects(voxel))
			{
				auto offset = m_hitbox.get_MTV(voxel);

				if (offset.x == 0. && offset.y == 0. && offset.z == 0.) continue;


				/*if (world.block_at(pos - types::voxel_pos{ offset.normal() }))
					offset = {};*/

				if (offset.y != 0)
					m_mov.velocity.y = 0;

				if (offset.x != 0)
					m_mov.velocity.x = 0;

				if (offset.z != 0)
					m_mov.velocity.z = 0;


				set_pos(get_pos() - offset);
			}
		}
	}
}
