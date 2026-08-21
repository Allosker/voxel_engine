#include "player.hpp"




void gfx::Player::move(Keys key, f64 dt) noexcept
{
	switch (key)
	{
	case Keys::W:
		m_mov.velocity += v3f64{m_cam->get_front().x, 0., m_cam->get_front().z} * m_mov.speed* dt;
		m_mov.moving_hor = true;
		break;

	case Keys::S:
		m_mov.velocity -= v3f64{ m_cam->get_front().x, 0., m_cam->get_front().z } * m_mov.speed * dt;
		m_mov.moving_hor = true;
		break;

	case Keys::A:
		m_mov.velocity -= v3f64{ m_cam->get_right().x, 0., m_cam->get_right().z } * m_mov.speed * dt;
		m_mov.moving_hor = true;
		break;

	case Keys::D:
		m_mov.velocity += v3f64{ m_cam->get_right().x, 0., m_cam->get_right().z } * m_mov.speed * dt;
		m_mov.moving_hor = true;
		break;


	case Keys::Space:
		m_mov.velocity += v3f64{ 0, m_cam->get_up().y, 0. } * m_mov.jump_height * dt;
		m_mov.moving_ver = true;
		break;

	case Keys::Left_shift:
		m_mov.velocity -= v3f64{ 0, m_cam->get_up().y, 0. } * m_mov.jump_height * dt;
		m_mov.moving_ver = true;
		break;
	}
}

void gfx::Player::update_position(f64 dt) noexcept
{

	if (v2f64{ m_mov.velocity.x, m_mov.velocity.z }.length_squared() > m_mov.max_speed * m_mov.max_speed)
	{
		const auto tempY{ m_mov.velocity.y };

		m_mov.velocity = v3f64{ m_mov.velocity.x, 0, m_mov.velocity.z }.normal() * m_mov.max_speed;
		m_mov.velocity.y = tempY; 
	}


	if (!m_mov.moving_hor)
	{
		m_mov.velocity.x *= (1 - m_mov.friction * dt);
		m_mov.velocity.z *= (1 - m_mov.friction * dt);
	}

	/*if (!m_mov.flying)
		m_mov.velocity.y += world.settings.gravity * deltaTime;*/

	

	set_pos(m_trans.get_pos() + m_mov.velocity * dt);


	if (!m_mov.moving_ver)
		m_mov.velocity.y = 0.;


	m_mov.moving_hor = false;
	m_mov.moving_ver = false;
}
