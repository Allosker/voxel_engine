#include "gfx/world.hpp"
#include "velocity.hxx"


namespace phy
{

	v3f64 phy::Velocity::update(const gfx::World& world, f64 dt) noexcept
	{
		if (glm::length2(v2f64{ m_velocity.x, m_velocity.z }) > max_speed * max_speed)
		{
			const auto tempY{ m_velocity.y };

			m_velocity = glm::normalize(v3f64{ m_velocity.x, 0, m_velocity.z }) * max_speed;
			m_velocity.y = tempY;
		}


		if (!m_moving_hor)
		{
			m_velocity.x *= (1 - friction * dt);
			m_velocity.z *= (1 - friction * dt);
		}

		if (!flying && !m_moving_ver)
			m_velocity.y += world.gravity * dt;
		else if (!m_moving_ver)
			m_velocity.y = 0.;

		m_moving_hor = false;
		m_moving_ver = false;

		return m_velocity * dt;
	}

	void Velocity::move_hor(const v2f64& dir, f64 dt) noexcept
	{
		m_velocity += glm::normalize(v3f64{ dir.x, 0., dir.y }) * acceleration * dt;
		m_moving_hor = true;
	}

	void Velocity::move_ver(f64 y, f64 dt) noexcept
	{
		if (y < 0)
		{
			if (flying)
			{
				if (m_velocity.y < 0.)
					m_velocity.y = 0.;

				m_velocity.y += y * acceleration * dt;
				m_moving_ver = true;
			}
			else if (m_is_on_ground)
			{
				m_velocity.y += y * jump_velocity;
			}
		}
		else
		{
			if (flying)
			{
				if (m_velocity.y > 0.)
					m_velocity.y = 0.;

				m_velocity -= v3f64{ 0, y, 0. } * acceleration * dt;
				m_moving_ver = true;
			}
		}

	}

}