#include "player.hpp"

#include <vector>

#include "chunk.hpp"
#include "voxelType.hpp"


void gfx::Player::move(Keys key, f64 dt) noexcept
{
	switch (key)
	{
	case Keys::W:
		velocity.accelerate(v3f64{ m_cam->get_front().x, 0., m_cam->get_front().z }, dt);
		break;

	case Keys::S:
		velocity.accelerate(-v3f64{ m_cam->get_front().x, 0., m_cam->get_front().z }, dt);
		break;

	case Keys::A:
		velocity.accelerate(v3f64{ m_cam->get_right().x, 0., m_cam->get_right().z }, dt);
		break;

	case Keys::D:
		velocity.accelerate(-v3f64{ m_cam->get_right().x, 0., m_cam->get_right().z }, dt);
		break;


	case Keys::Space:
		if (bounds.is_on_ground)
			velocity.accelerate(v3f64{ 0., m_cam->get_up().y * jump_velocity / velocity.settings.acceleration, 0. }, dt);
		else
			velocity.accelerate(v3f64{ 0., m_cam->get_up().y, 0. }, dt); 
		break;

	case Keys::Left_shift:
		velocity.accelerate(-v3f64{ 0., m_cam->get_up().y, 0. }, dt);
		break;

	}
}

void gfx::Player::resolve_collisions_entities(World& world, PlayerInventory& inv, f64 dt) noexcept
{
	auto hitbox = m_hitbox;
	hitbox.move(get_pos());
	if (debug.show_hitbox)
		aabb_min_max((v3f32)hitbox.get_min(), (v3f32)hitbox.get_max(), { 1, 0, 0, 1 }, 0., false);


	const auto floored_pos_min = World::to_voxelPos(hitbox.get_min());
	const auto floored_pos_max = World::to_voxelPos(hitbox.get_max());

	world.remove_entities_if(World::to_chunkLoc(get_pos()), [&](const WorldItem& item)
	{
		if (phy::intersects(hitbox, item.get_hitbox()))
		{
			inv.get_inventory().add_items({ item.get_id(), {} }, 1);
			return true;
		}
		return false;
	});
	
}

void gfx::Player::resolve_collisions_world(World& world, f64 dt) noexcept
{
	bounds.is_on_ground = false;

	if (bounds.ghost)
		return;

	std::vector<types::voxel_pos> voxel_positions;

	auto hitbox = m_hitbox;
	hitbox.move(get_pos());
	if (debug.show_hitbox)       
		aabb_min_max((v3f32)hitbox.get_min(), (v3f32)hitbox.get_max(), { 1, 0, 0, 1 }, 0., false);


	const Chunk* chunk = nullptr;

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
			aabb_min_max((v3f32)voxel.get_min(), (v3f32)voxel.get_max(), { 1, 1, 1, 1 }, 0., false);
					

			if (phy::intersects(hitbox, voxel))
			{
				auto offset = phy::get_MTV(hitbox, voxel);

				if (offset.x == 0. && offset.y == 0. && offset.z == 0.) continue;


				if (VoxelTypeManager::get().get_type(world.get_voxel(World::to_voxelPos(static_cast<types::pos>(pos) - glm::normalize(offset)))).has_bounds)
					offset = {};

				velocity.set(v3f64{ offset.x ? 0 : velocity.get().x, offset.y ? 0 : velocity.get().y, offset.z ? 0 : velocity.get().z });

				if (offset.y < 0)
					bounds.is_on_ground = true;

				set_pos(get_pos() - offset);
			}
		}
	}
}
