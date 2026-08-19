#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Define a basic struct that any physic object can inherit from and be treated by the engine
* ==============================================-
*/


#include "transformable3D.hpp"

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


	private:

		HitboxAABB m_hitbox;
		PhysicsMaterial m_material;


	};

}