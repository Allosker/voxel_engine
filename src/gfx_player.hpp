#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Define the player, at the center of the world, it manages all in-world-user related states
* ==============================================-
*/

#include "sys_types.hpp"

#include "gfx_camera.hpp"
#include "gfx_world.hpp"


namespace gfx
{
	
	class Player
	{
	public:


		void set_camera(Camera* cam) noexcept
		{

		}



	private:

		/// <summary>
		/// Allow the player to change camera (for cinematics)
		/// </summary>
		Camera* m_cam{ nullptr };


	};


}