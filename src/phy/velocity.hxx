#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Contains all necessary information about velocity, as well as a simple interface for calculating it
* ==============================================-
*/

#include "sys/types.hpp"

namespace gfx
{
	class World;
}

namespace phy
{
	

	class Velocity
	{
	public:

		Velocity() noexcept = default;

		/// <summary>
		/// Calculates current velocity
		/// </summary>
		/// <param name="dt"></param>
		/// <returns>The velocity * dt</returns>
		v3f64 update(const gfx::World& world, f64 dt) noexcept;

		/// <summary>
		/// Updates the velocity on the x/y axis
		/// </summary>
		/// <param name="Direction in which to move"></param>
		/// <param name="dt"></param>
		void move_hor(const v2f64& dir, f64 dt) noexcept;
		/// <summary>
		/// Updates the velocity on the y axis
		/// </summary>
		/// <param name="y"></param>
		/// <param name="dt"></param>
		void move_ver(f64 y, f64 dt) noexcept;


	public:

		f64 acceleration{ 20. };
		f64 jump_velocity{ 8.4 };

		f64 max_speed{ 5. };

		bool flying{ true };
		bool ghost{ true };


	private:

		v3f64 m_velocity{};

		f64 friction{ 10. };

		bool m_is_on_ground{};		
		bool m_moving_hor{};
		bool m_moving_ver{};

	};


}