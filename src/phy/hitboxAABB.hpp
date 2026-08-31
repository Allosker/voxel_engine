#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Small implementation of an AABB that can be created on the fly
* ==============================================-
*/

#include "sys/types.hpp"


namespace phy
{
	class HitboxAABB;

	/// <summary> Consider two cases:
	/// <para>- Intersecting -> MTV </para>
	/// <para>- Not Intersecting -> empty vector </para>
	/// </summary>
	/// <returns>The MTV (minimum translation vector) to stop intersecting with the current AABB</returns>
	bool intersects(const HitboxAABB& a, const HitboxAABB& b) noexcept;

	/// <summary>
	/// Exclusive bounds, we do not wish to consider two overlapping hitboxes to be intersecting 
	/// </summary>
	/// <param name="other"></param>
	/// <returns>Whether the two hitboxes intersect</returns>
	v3f64 get_MTV(const HitboxAABB& a, const HitboxAABB& b) noexcept;


	/// <summary>
	/// Basic AABB hitbox that considers an origin and a size, positive direction goes towards +inf
	/// <para>Note: the position is considered to be the middle point</para>
	/// </summary>
	class HitboxAABB
	{
	public:

		HitboxAABB(const types::pos& pos, const v3f64& extent = v3f64{ 1. }) noexcept
			: m_pos{ pos }, m_extent{extent}
		{}

		DEFAULT_COPY_INIT(HitboxAABB);
		DEFAULT_MOVE_INIT(HitboxAABB);

		HitboxAABB() noexcept = delete;

		~HitboxAABB() noexcept = default;


		v3f64 get_extent() const noexcept { return m_extent; }

		v3f64 get_pos() const noexcept { return m_pos; }

		v3f64 get_min() const noexcept { return m_pos - m_extent; }
		v3f64 get_max() const noexcept { return m_pos + m_extent; }


		void set_pos(const types::pos& pos) noexcept
		{
			m_pos = pos;
		}

		void set_extent(const v3f64& extent) noexcept
		{
			m_extent = extent;
		}


		void move(const types::pos& offset) noexcept
		{
			set_pos(get_pos() + offset);
		}


	private:

		v3f64 m_pos{};
		v3f64 m_extent{};


	};


}