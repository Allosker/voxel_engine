#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Simple Ray Traversal algorithm
* ==============================================-
*/

#include <optional>

#include "sys/types.hpp"

#include "chunkGrid.hpp"
#include "voxelType.hpp"
#include "world.hpp"


namespace gfx
{
	struct Ray
	{
	public:

		// = Initialization

		Ray() noexcept = default;

		Ray(const types::pos& origin_, const types::pos& dir) noexcept
			: pos{ origin_ }, step{ 1 }, nextBound{ std::floor(pos.x), std::floor(pos.y), std::floor(pos.z) }
		{
			if (dir.x < 0)
				step.x = -1;
			if (dir.y < 0)
				step.y = -1;
			if (dir.z < 0)
				step.z = -1;

			// Next Bound
			if (dir.x > 0)
				nextBound.x++;
			if (dir.y > 0)
				nextBound.y++;
			if (dir.z > 0)
				nextBound.z++;


			tMax.x = (nextBound.x - pos.x) / dir.x;
			tMax.y = (nextBound.y - pos.y) / dir.y;
			tMax.z = (nextBound.z - pos.z) / dir.z;

			tDelta.x = 1 / std::abs(dir.x);
			tDelta.y = 1 / std::abs(dir.y);
			tDelta.z = 1 / std::abs(dir.z);
		}

		// = Members

		types::pos	pos{};
		types::pos	step{};
		types::pos	nextBound{};
		types::pos	tMax{};
		types::pos	tDelta{};

	};

	/// <summary>
	/// The result of a Ray Cast, containing the origin of the ray, its current position, and its normal
	/// </summary>
	struct RayCastResult
	{
		types::pos			origin{};
		types::voxel_pos	voxel_pos{};
		types::pos			hit_pos{};
		types::pos			normal{};
	};

	
	/// <summary>
	/// Cast a ray using the Ray Traversal algorithm, if the ray hits a solid block, the ray cast stops
	/// </summary>
	/// <param name="origin"></param>
	/// <param name="direction"></param>
	/// <param name="chunk grid"></param>
	/// <param name="max length of ray"></param>
	/// <returns>std::nullopt if the Ray hit nothing, the RayCastResult otherwise</returns>
	static inline std::optional<RayCastResult> raycast(const types::pos& origin, const types::pos& dir, const gfx::ChunkGrid& grid, u64 max_length) noexcept
	{
		Ray ray{ origin, dir };
		types::pos normal{};
		double tMin{};

		types::chunk_loc ray_loc{ World::to_chunkLoc(origin) };
		types::chunk_loc old_loc{};

		const Chunk* chunk{ nullptr };

		if (!(chunk = grid.at_chunk(ray_loc)))
			return std::nullopt;

		while (glm::length2(origin - ray.pos) < max_length * max_length)
		{

			ray_loc = World::to_chunkLoc(ray.pos);

			if (old_loc != ray_loc)
			{
				old_loc = ray_loc;

				chunk = grid.at_chunk(ray_loc);

				if (!chunk)
					return std::nullopt;
			}

			const auto vloc = chunk->to_voxelLoc(*chunk, World::to_voxelPos(ray.pos));

			if (auto* v = chunk->at_ptr(vloc))
				if (VoxelTypeManager::get().get_type(v->type_id).is_solid)
				{
					tMin += 0.0001f;
					const auto hitPos = origin + dir * tMin;
					return std::make_optional<RayCastResult>(origin, World::to_voxelPos(ray.pos), hitPos, normal);
				}

			if (ray.tMax.x < ray.tMax.y)
			{
				if (ray.tMax.x < ray.tMax.z)
				{
					tMin = ray.tMax.x;
					ray.pos.x += ray.step.x;
					ray.tMax.x += ray.tDelta.x;
					normal = { -ray.step.x, 0., 0. };
				}
				else
				{
					tMin = ray.tMax.z;
					ray.pos.z += ray.step.z;
					ray.tMax.z += ray.tDelta.z;
					normal = { 0., 0., -ray.step.z };
				}
			}
			else
			{
				if (ray.tMax.y < ray.tMax.z)
				{
					tMin = ray.tMax.y;
					ray.pos.y += ray.step.y;
					ray.tMax.y += ray.tDelta.y;
					normal = { 0., -ray.step.y, 0. };
				}
				else
				{
					tMin = ray.tMax.z;
					ray.pos.z += ray.step.z;
					ray.tMax.z += ray.tDelta.z;
					normal = { 0., 0., -ray.step.z };
				}
			}
		}

		return std::nullopt;
	}
}