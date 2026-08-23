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

	/// <summary>
	/// Basic AABB hitbox that considers an origin and a size, positive direction goes towards +inf
	/// <para>Note: the origin is considered to be the middle point</para>
	/// </summary>
	class HitboxAABB
	{
	public:

		HitboxAABB(const types::pos& ori, const v3f64& extent = { 1. }) noexcept
			: m_extent{ extent }, m_min{ ori - m_extent }, m_max{ ori + m_extent }
		{}

		DEFAULT_COPY_INIT(HitboxAABB);
		DEFAULT_MOVE_INIT(HitboxAABB);

		HitboxAABB() noexcept = delete;

		~HitboxAABB() noexcept = default;


		v3f64 get_extent() const noexcept { return m_extent; }

		v3f64 get_pos() const noexcept { return m_min + get_extent(); }

		v3f64 get_min() const noexcept { return m_min; }
		v3f64 get_max() const noexcept { return m_max; }


		void set_pos(const types::pos& pos) noexcept
		{
			m_min = pos - get_extent();
			m_max = pos + get_extent();
		}

		void set_extent(const v3f64& extent) noexcept
		{
			m_min = m_min + get_extent() - extent;
			m_max = m_max - get_extent() + extent;

			m_extent = extent;
		}


		void move(const types::pos& offset) noexcept
		{
			set_pos(get_pos() + offset);
		}

		/// <summary>
		/// Exclusive bounds, we do not wish to consider two overlapping hitboxes to be intersecting 
		/// </summary>
		/// <param name="other"></param>
		/// <returns>Whether the two hitboxes intersect</returns>
		bool intersects(const HitboxAABB& other) const noexcept
		{
			return
				m_min.x < other.m_max.x && m_max.x > other.m_min.x &&

				m_min.y < other.m_max.y && m_max.y > other.m_min.y &&

				m_min.z < other.m_max.z && m_max.z > other.m_min.z;
		}

		/// <summary> Consider two cases:
		/// <para>- Intersecting -> MTV </para>
		/// <para>- Not Intersecting -> empty vector </para>
		/// </summary>
		/// <returns>The MTV (minimum translation vector) to stop intersecting with the current AABB</returns>
		v3f64 get_MTV(const HitboxAABB& other) const noexcept
		{
			const f64 right{ m_max.x - other.m_min.x };
			const f64 down{ m_max.y - other.m_min.y };
			const f64 back{ m_max.z - other.m_min.z };

			const f64 left{ other.m_max.x - m_min.x };
			const f64 up{ other.m_max.y - m_min.y };
			const f64 front{ other.m_max.z - m_min.z };

			v3f64 ret{};
			f64 bestDist{ -1 };


			if (left > 0 && (left < bestDist || bestDist < 0))
			{
				bestDist = left;
				ret = -v3f64{ left, 0, 0 };
			}

			if (right > 0 && (right < bestDist || bestDist < 0))
			{
				bestDist = right;
				ret = v3f64{ right, 0, 0 };
			}


			if (up > 0 && (up < bestDist || bestDist < 0))
			{
				bestDist = up;
				ret = -v3f64{ 0, up, 0 };
			}

			if (down > 0 && (down < bestDist || bestDist < 0))
			{
				bestDist = down;
				ret = v3f64{ 0, down, 0 };
			}


			if (front > 0 && (front < bestDist || bestDist < 0))
			{
				bestDist = front;
				ret = -v3f64{ 0, 0, front };
			}

			if (back > 0 && (back < bestDist || bestDist < 0))
			{
				bestDist = back;
				ret = v3f64{ 0, 0, back };
			}


			return ret;
		}
		


	private:


		v3f64 m_min{};
		v3f64 m_max{};

		v3f64 m_extent{};


	};


}