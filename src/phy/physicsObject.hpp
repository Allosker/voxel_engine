#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Define a basic Physics Object with specific attributes that can interact with other physics objects
* ==============================================-
*/


#include "gfx/transformable3D.hpp"

#include "hitboxAABB.hpp"


namespace phy
{

	struct PhysicsMaterial
	{
		f64 occupancy;
		f64 bounciness;
		f64 friction;
	};

	class PhysicsObject
	{
	public:

		PhysicsObject(const types::pos& pos, const v3f64& size, const PhysicsMaterial& material)
			: m_hitbox{ pos, size }, m_material{ material }
		{
		}

		DEFAULT_COPY_INIT(PhysicsObject);
		DEFAULT_MOVE_INIT(PhysicsObject);


	private:

		HitboxAABB m_hitbox;
		PhysicsMaterial m_material;


	};

}