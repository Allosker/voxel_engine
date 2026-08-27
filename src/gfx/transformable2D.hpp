#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	2D transformable that creates a 4x4 model matrix as a result of any translation/scaling/rotation it undergoes
* Mainly for any game object that needs transformations
* 
* Note: The resulting model matrix is 4x4 for simpler computations, as shaders directly deal with 4x4 matrices anyway.
* ==============================================-
*/

#include "sys/types.hpp"


namespace gfx
{

	class Transformable2D
	{
	public:

		Transformable2D(v2f32 size = {}, types::pos2d ori = {})
			: m_origin{ ori }, m_baseSize{ size }, m_scale{ 1.f, 1.f }
		{
			if (m_origin.x != 0 || m_origin.y != 0)
				m_transformNeedUpdate = true;
		}

		Transformable2D(Transformable2D&&) = default;
		Transformable2D(Transformable2D&) = default;

		Transformable2D& operator=(Transformable2D&&) = default;
		Transformable2D& operator=(Transformable2D&) = default;

		~Transformable2D() = default;


		// = Getters

		const m4f32& get_transform() noexcept
		{
			// Recompute the combined transform if needed
			if (m_transformNeedUpdate)
			{
				const float angle = -m_rotation.as_radians();
				const float cosine = std::cos(angle);
				const float sine = std::sin(angle);
				const float sxc = m_scale.x * cosine;
				const float syc = m_scale.y * cosine;
				const float sxs = m_scale.x * sine;
				const float sys = m_scale.y * sine;
				const float tx = -m_origin.x * sxc - m_origin.y * sys + m_position.x;
				const float ty = m_origin.x * sxs - m_origin.y * syc + m_position.y;

				m_transformations = m4f32
				{
					sxc ,	sxs,	0.f, 0.f,
					-sys,	syc,	0.f, 0.f,
					0.f ,	0.f,	1.f, 0.f,
					tx  ,	ty ,	0.f, 1.f
				};

				m_transformNeedUpdate = false;
			}

			return m_transformations;
		}

		v2f32 get_size() const noexcept { return { m_baseSize.x * m_scale.x, m_baseSize.y * m_scale.y }; }

		v2f32 get_scale() const noexcept { return m_scale; }

		v2f32 get_base_size() const noexcept { return { m_baseSize.x, m_baseSize.y }; }

		angle32 get_rotation() const noexcept { return m_rotation; }

		types::pos2d get_pos() const noexcept { return m_position; }

		v2f32 get_ori() const noexcept { return m_origin; }
	

		// = Setters

		virtual void set_pos(types::pos2d pos) noexcept
		{
			m_position = pos;
			m_transformNeedUpdate = true;
		}

		virtual void set_scale(v2f32 scale) noexcept
		{
			m_scale = scale;
			m_transformNeedUpdate = true;
		}

		virtual void set_size(v2f32 size) noexcept
		{
			if (m_baseSize != 0)
				set_scale({ size.x / m_baseSize.x, size.y / m_baseSize.y });
			else
				m_baseSize = size;
			m_transformNeedUpdate = true;
		}

		virtual void set_base_size(v2f32 size) noexcept
		{
			m_baseSize = size;
			m_transformNeedUpdate = true;
		}

		virtual void set_ori(types::pos2d ori) noexcept
		{
			m_origin = ori;
			m_transformNeedUpdate = true;
		}

		virtual void set_rotation(angle32 rotation) noexcept
		{
			m_rotation = rotation;
			m_transformNeedUpdate = true;
		}


		void move(v2f32 offset) noexcept
		{
			set_pos(m_position + offset);
		}

		void scale(v2f32 factor) noexcept
		{
			set_scale({ m_scale.x * factor.x, m_scale.y * factor.y });
		}

		void rotate(angle32 theta) noexcept
		{
			set_rotation(m_rotation + theta);
		}


	private:

		m4f32 m_transformations{ m4f32::Identity };

		v2f32			m_scale{};
		v2f32			m_baseSize{};
		v2f32			m_origin{};
		types::pos2d	m_position{};
						
		angle32			m_rotation{ angle32::from_radians(0) };
						
		bool			m_transformNeedUpdate{ false };

	};

}