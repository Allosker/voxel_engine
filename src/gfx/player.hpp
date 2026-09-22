#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Define the player, at the center of the world, it manages all in-world-user related states
* ==============================================-
*/

#include "sys/inputTypes.hpp"
#include "sys/types.hpp"

#include "camera.hpp"
#include "world.hpp"

#include "transformable3D.hpp"

#include "phy/hitboxAABB.hpp"

#include "debugRenderer.hpp"
#include "gfx/playerInventory.hpp"
#include "phy/velocity.hxx"


namespace gfx
{
	
	class Player
	{
	public:

		Player(Camera* cam)
			: m_cam{ cam }, m_hitbox{ v3f64{ 0., -0.8, 0. }, { 0.25, 0.9, 0.25 }},
			velocity{
				&m_mat, phy::VelocitySettings{.acceleration{ 20 }, .max_speed{ 5. } }, true
			}
		{ }


		const types::pos& get_pos() const noexcept { return m_trans.get_pos(); }

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

		void update(World& world, PlayerInventory& inv, f64 dt) noexcept
		{
			set_pos(m_trans.get_pos() + velocity.update(world.gravity, dt));

			resolve_collisions_world(world, dt);
			resolve_collisions_entities(world, inv, dt);
		}


	private:

		void update_position(World& world, f64 dt) noexcept;

		void resolve_collisions_world(World& world, f64 dt) noexcept;

		void resolve_collisions_entities(World& world, PlayerInventory& inv, f64 dt) noexcept;


	private:

		/// <summary>
		/// Allow the player to change camera (for cinematics)
		/// Is considered to always be valid
		/// </summary>
		Camera* m_cam{ nullptr };

		Transformable3D m_trans{};

		phy::HitboxAABB m_hitbox;

		phy::PhysicsMaterial m_mat{ .friction{ 10. } };


	public: // for debug

		struct BoundChecking
		{
			bool ghost{ true };
			bool is_on_ground{};
		} bounds;
		f64 jump_velocity{ 8.4 };

		phy::Velocity velocity;
	

		struct Debug
		{
			bool show_hitbox{};
		} debug;


	};


}