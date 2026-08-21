#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Define the player, at the center of the world, it manages all in-world-user related states
* ==============================================-
*/

#include "sys/types.hpp"
#include "sys/inputTypes.hpp"

#include "camera.hpp"
#include "world.hpp"

#include "transformable3D.hpp"


namespace gfx
{
	
	class Player
	{
	public:

		Player(Camera* cam)
			: m_cam{ cam }
		{ }


		void set_camera(Camera* cam) noexcept
		{
			m_cam = cam;
		}

		void set_pos(const types::pos& new_pos) noexcept
		{
			m_trans.set_pos(new_pos);

			m_cam->set_pos(m_trans.get_pos());
		}


		void move(Keys key, f64 dt) noexcept;

		void update(f64 dt) noexcept
		{
			update_position(dt);
		}


	private:

		void update_position(f64 dt) noexcept;


	private:

		/// <summary>
		/// Allow the player to change camera (for cinematics)
		/// Is considered to always be valid
		/// </summary>
		Camera* m_cam{ nullptr };

		Transformable3D m_trans{};


	public: // for debug

		struct Movement
		{
			v3f64 velocity{};

			f64 speed{ 20. };
			f64 jump_height{ 20. };

			f64 max_speed{ 5. };

			f64 friction{ 10. };

			bool flying{ true };
			bool ghost{};
			bool moving_hor{};
			bool moving_ver{};

		} m_mov;
	

	};


}