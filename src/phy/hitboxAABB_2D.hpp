#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Small implementation of a 2D AABB that can be created on the fly
* ==============================================-
*/

#include "sys/types.hpp"


namespace phy
{

	class HitboxAABB2D;

	/// <summary> Consider two cases:
	/// <para>- Intersecting -> MTV </para>
	/// <para>- Not Intersecting -> empty vector </para>
	/// </summary>
	/// <returns>The MTV (minimum translation vector) to stop intersecting with the current AABB</returns>
	bool intersects(const HitboxAABB2D& a, const HitboxAABB2D& b) noexcept;

	/// <summary>
	/// Exclusive bounds, we do not wish to consider two overlapping hitboxes to be intersecting 
	/// </summary>
	/// <param name="other"></param>
	/// <returns>Whether the two hitboxes intersect</returns>
	v2f32 get_MTV(const HitboxAABB2D& a, const HitboxAABB2D& b) noexcept;


	/// <summary>
	/// Basic AABB hitbox that considers an origin and a size, positive direction goes towards +inf
	/// <para>Note: the position is considered to be the middle point</para>
	/// </summary>
	class HitboxAABB2D
	{
	public:

		HitboxAABB2D(types::pos2d pos, v2f32 extent = { 1. }) noexcept
			: m_pos{ pos }, m_extent{ extent }
		{
		}

		DEFAULT_COPY_INIT(HitboxAABB2D);
		DEFAULT_MOVE_INIT(HitboxAABB2D);

		HitboxAABB2D() noexcept = delete;

		~HitboxAABB2D() noexcept = default;


		v2f32 get_extent() const noexcept { return m_extent; }

		types::pos2d get_pos() const noexcept { return m_pos; }

		v2f32 get_min() const noexcept { return m_pos - m_extent; }
		v2f32 get_max() const noexcept { return m_pos + m_extent; }


		void set_pos(types::pos2d pos) noexcept
		{
			m_pos = pos;
		}

		void set_extent(v2f32 extent) noexcept
		{
			m_extent = extent;
		}


		void move(types::pos2d offset) noexcept
		{
			set_pos(get_pos() + offset);
		}


	private:

		types::pos2d m_pos{};
		v2f32 m_extent{};


	};


}