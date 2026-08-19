#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Define the player, at the center of the world, it manages all in-world-user related states
* ==============================================-
*/

#include "sys/types.hpp"

#include "camera.hpp"
#include "world.hpp"

#include "transformable3D.hpp"


namespace gfx
{
	
	class Player
	{
	public:


		void set_camera(Camera* cam) noexcept
		{

		}

		void set_pos(const types::pos& new_pos) noexcept
		{
			m_trans.set_pos(new_pos);

			if (m_cam)
				m_cam->set_pos(new_pos);
		}


	private:

		/// <summary>
		/// Allow the player to change camera (for cinematics)
		/// </summary>
		Camera* m_cam{ nullptr };

		Transformable3D m_trans{};


	};


}