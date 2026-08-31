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

		Transformable2D(v2f32 scale = { 1.f, 1.f }, types::pos2d ori = {})
			: m_origin{ ori }, m_scale{ scale }
		{
			if (m_origin.x != 0 || m_origin.y != 0)
				m_transformNeedUpdate = true;
		}

		Transformable2D(Transformable2D&&) = default;
		Transformable2D(Transformable2D&) = default;

		Transformable2D& operator=(Transformable2D&&) = default;
		Transformable2D& operator=(Transformable2D&) = default;

		~Transformable2D() = default;



		const m4f32& get_transform() noexcept
		{
			if (m_transformNeedUpdate)
			{
				const f32 angle = -glm::radians(m_rotation);
				const f32 cosine = std::cos(angle);
				const f32 sine = std::sin(angle);
				const f32 sxc = m_scale.x * cosine;
				const f32 syc = m_scale.y * cosine;
				const f32 sxs = m_scale.x * sine;
				const f32 sys = m_scale.y * sine;
				const f32 tx = -m_origin.x * sxc - m_origin.y * sys + m_position.x;
				const f32 ty = m_origin.x * sxs - m_origin.y * syc + m_position.y;

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


		v2f32 get_scale() const noexcept { return m_scale; }

		angle get_rotation() const noexcept { return m_rotation; }

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

		virtual void set_scale(f32 scalar) noexcept
		{
			set_scale({ scalar, scalar });
		}

		virtual void set_ori(types::pos2d ori) noexcept
		{
			m_origin = ori;
			m_transformNeedUpdate = true;
		}

		virtual void set_rotation(angle rotation) noexcept
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

		void rotate(angle theta) noexcept
		{
			set_rotation(m_rotation + theta);
		}


	private:

		m4f32 m_transformations{ 1 };

		v2f32			m_scale{};
		v2f32			m_origin{};
		types::pos2d	m_position{};
						
		angle			m_rotation{};
						
		bool			m_transformNeedUpdate{ false };

	};

}