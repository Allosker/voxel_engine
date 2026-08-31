#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Define a basic camera that holds its view/proj matrices as a single one
* ==============================================-
*/

#include "sys/types.hpp"


namespace gfx
{
	struct Camera
	{
		Camera(types::pos pos_Start) noexcept
			: m_pos{ pos_Start }
		{
		}

		Camera() noexcept = default;


		// Setters

		void set_dirs(const types::pos& front) noexcept
		{
			m_frontdir = front;

			build_cam_matrix();
		}

		void set_pos(const types::pos& position) noexcept
		{
			m_pos = position;

			build_cam_matrix();
		}

		/// <summary>
		/// Set the size of the frame buffer for building projection matrices
		/// </summary>
		/// <param name="new_size"></param>
		void set_FBS(v2f32 new_size) noexcept
		{
			if (new_size.x == 0 || new_size.y == 0)
				return;

			m_framebuffer_size = new_size;

			build_cam_matrix();
		}

		void set_FOV(const f64 fov) noexcept
		{
			m_fov = fov;
			build_cam_matrix();
		}


		// = Actors

		void toggle_dir_updates() noexcept { m_update_dirs = !m_update_dirs; }

		// Absolute
		void move_up(f64 dt, f64 speed) noexcept
		{
			set_pos(m_pos + m_updir * dt * speed);
		}

		void move_down(f64 dt, f64 speed) noexcept
		{
			set_pos(m_pos - m_updir * dt * speed);
		}

		void move_right(f64 dt, f64 speed) noexcept
		{
			set_pos(m_pos + glm::cross(m_frontdir, m_updir) * dt * speed);
		}

		void move_left(f64 dt, f64 speed) noexcept
		{
			set_pos(m_pos - glm::cross(m_frontdir, m_updir) * dt * speed);
		}

		void move_front(f64 dt, f64 speed) noexcept
		{
			set_pos(m_pos + m_frontdir * dt * speed);
		}

		void move_back(f64 dt, f64 speed) noexcept
		{
			set_pos(m_pos - m_frontdir * dt * speed);
		}


		// = Getters

		types::pos get_right() const noexcept { return glm::cross(m_frontdir, m_updir); }

		const types::pos& get_up() const noexcept { return m_updir; }

		const types::pos& get_front() const noexcept { return m_frontdir; }

		const types::pos& get_pos() const noexcept { return m_pos; }

		/// <summary>
		/// View projection matrix used to build the projection plan
		/// </summary>
		/// <returns>View Projection matrix</returns>
		const m4f64& get_VP() const noexcept { return m_vp; }

		const bool should_update_dirs() const noexcept { return m_update_dirs; }


		f32 speed{10.f};

	private:

		void build_cam_matrix() noexcept
		{
			m4f64 view = glm::lookAt(m_pos, m_frontdir + m_pos, m_updir);
			m4f64 proj = glm::perspective<f64>(glm::radians(m_fov), m_framebuffer_size.x / m_framebuffer_size.y, 0.1f, 1000.f);

			m_vp = proj * view;
		}


		m4f64 m_vp{};

		f64 m_fov{ 45 };

		v2f32 m_framebuffer_size{};
		types::pos  m_pos{ 0.f, 0.f, 0.f };
		types::pos  m_updir{ 0.f, 1.f, 0.f };
		types::pos  m_frontdir{ 0.f, 0.f, -1.f };

		bool m_update_dirs{ false };
	};

}