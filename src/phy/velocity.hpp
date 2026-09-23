#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Small module for simple velocity calculations
* ==============================================-
*/

#include "sys/types.hpp"


namespace phy
{
	struct MovementSettings
	{
		v3f64 velocity{};

		f64 acceleration{};

		f64 max_speed{};

		f64 friction{};

		bool moving_hor{};
		bool moving_ver{};

	};

	
	inline v3f64 calculate_velocity(const MovementSettings& set, f64 dt, f64 gravity, bool compute_gravity = true) noexcept
	{
		v3f64 velocity{ set.velocity };

		if (glm::length2(v2f64{ velocity.x, velocity.z }) > set.max_speed * set.max_speed)
		{
			const auto tempY{ velocity.y };

			velocity = glm::normalize(v3f64{ velocity.x, 0, velocity.z }) * set.max_speed;
			velocity.y = tempY;
		}


		if (!set.moving_hor)
		{
			velocity.x *= (1 - set.friction * dt);
			velocity.z *= (1 - set.friction * dt);
		}

		if (compute_gravity)
			velocity.y += gravity * dt;


		return velocity;
	}


}