#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Define a basic camera that holds its view/proj matrices as a single one
* ==============================================-
*/

#include "sys_types.hpp"


namespace gfx
{
	struct Camera
	{
		Camera(v3f32 pos_Start) noexcept
			: m_pos{ pos_Start }
		{
		}

		Camera() noexcept = default;


		// Setters

		void setDirections(const v3f32& front) noexcept
		{
			m_frontdir = front;

			build_cam_matrix();
		}

		void setPosition(const v3f32& position) noexcept
		{
			m_pos = position;

			build_cam_matrix();
		}

		void setFramebufferSize(const v2f32& new_size) noexcept
		{
			m_framebuffer_size = new_size;

			build_cam_matrix();
		}

		void setFOV(const float fov) noexcept
		{
			m_fov = fov;
			build_cam_matrix();
		}


		// = Actors

		void toggle_dir_updates() noexcept { m_update_dirs = !m_update_dirs; }

		// Absolute
		void move_up(float dt, float speed) noexcept
		{
			setPosition(m_pos + m_updir * dt * speed);
		}

		void move_down(float dt, float speed) noexcept
		{
			setPosition(m_pos - m_updir * dt * speed);
		}

		void move_right(float dt, float speed) noexcept
		{
			setPosition(m_pos + m_frontdir.cross(m_updir) * dt * speed);
		}

		void move_left(float dt, float speed) noexcept
		{
			setPosition(m_pos - m_frontdir.cross(m_updir) * dt * speed);
		}

		void move_front(float dt, float speed) noexcept
		{
			setPosition(m_pos + m_frontdir * dt * speed);
		}

		void move_back(float dt, float speed) noexcept
		{
			setPosition(m_pos - m_frontdir * dt * speed);
		}


		// = Getters

		v3f32 getRightDir() const noexcept { return m_frontdir.cross(m_updir); }

		const v3f32& getUpDir() const noexcept { return m_updir; }

		const v3f32& getFrontDir() const noexcept { return m_frontdir; }

		const v3f32& getPosition() const noexcept { return m_pos; }

		const m4f32& getViewProj() const noexcept { return m_vp; }

		const bool should_update_dirs() const noexcept { return m_update_dirs; }


		f32 speed{};

	private:

		void build_cam_matrix() noexcept
		{
			m4f32 view = mpml::lookAt(m_pos, m_frontdir + m_pos, m_updir);
			m4f32 proj = mpml::perspective(mpml::Angle<>::from_degrees(m_fov), m_framebuffer_size.x, m_framebuffer_size.y, 0.1f, 1000.f);

			m_vp = view * proj;
		}


		m4f32 m_vp{};

		float m_fov{ 45 };

		v2f32 m_framebuffer_size{};
		v3f32 m_pos{ 0.f, 0.f, 0.f };
		v3f32 m_updir{ v3f32{0.f, 1.f, 0.f} };
		v3f32 m_frontdir{ 0.f, 0.f, -1.f };

		bool m_update_dirs{ false };
	};

}