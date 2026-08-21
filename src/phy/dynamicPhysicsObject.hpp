#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Dynamic physics objects are physics objects that can move/rotate/scale
* ==============================================-
*/

#include "physicsObject.hpp"

namespace phy
{

	class DynamicPhysicsObject
	{
	public:

		struct Movement
		{
			v3f64 velocity{};
			v3f64 force{};

			f64 speed{};
			f64 mass{};

			f64 max_speed{};
		};


	public:

		DynamicPhysicsObject(const PhysicsObject& obj, const Movement& movement)
			: m_obj{ obj }, m_movement{ movement }
		{ 
		}

		DEFAULT_COPY_INIT(DynamicPhysicsObject);
		DEFAULT_MOVE_INIT(DynamicPhysicsObject);


		void move(const v3f64& dir) noexcept
		{
			m_movement.velocity += dir * m_movement.speed;
		}


	private:

		PhysicsObject m_obj;

		Movement m_movement;


	};

}