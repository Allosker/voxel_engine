#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	3D transformable that creates a 4x4 model matrix as a result of any translation/scaling/rotation it undergoes
* Mainly for any game object that needs transformations
* ==============================================-
*/

#include "sys/types.hpp"


namespace gfx
{

	class Transformable3D
	{
	public:

		Transformable3D(const v3f64& scale = { 1., 1., 1. }, const types::pos& ori = {})
			: m_origin{ ori }, m_scale{ scale }
		{
			if (m_origin.x != 0 || m_origin.y != 0)
				m_transformNeedUpdate = true;
		}

		Transformable3D(Transformable3D&&) = default;
		Transformable3D(Transformable3D&) = default;

		Transformable3D& operator=(Transformable3D&&) = default;
		Transformable3D& operator=(Transformable3D&) = default;

		~Transformable3D() = default;


		// = Getters

		const m4f64& get_transform() const noexcept
		{
			if (m_transformNeedUpdate)
			{
				m4f64 transforms{ 1. };

				transforms = glm::scale(glm::translate(m4f64{ 1. }, m_position) * glm::mat4_cast(m_rotation), m_scale);

				m_transformations = transforms;

				m_transformNeedUpdate = false;
			}

			return m_transformations;
		}

		qf64 get_rotation() const noexcept { return m_rotation; }

		const types::pos& get_pos() const noexcept { return m_position; }

		const v3f64& get_scale() const noexcept { return m_scale; }


		// = Setters

		void set_pos(const types::pos& pos) noexcept
		{
			m_position = pos;
			m_transformNeedUpdate = true;
		}

		void set_scale(const v3f64& scale) noexcept
		{
			m_scale = scale;
			m_transformNeedUpdate = true;
		}

		void set_scale(f64 scale) noexcept
		{
			set_scale({ scale, scale, scale });
		}

		void set_rotation(const qf64& q) noexcept
		{
			m_rotation = q;
			m_transformNeedUpdate = true;
		}

		void move(const v3f64& offset) noexcept
		{
			set_pos(m_position + offset);
		}

		void scale(const v3f64& factor) noexcept
		{
			set_scale({ m_scale.x * factor.x, m_scale.y * factor.y, m_scale.z * factor.z });
		}

		void scale(f64 factor) noexcept
		{
			set_scale({ m_scale.x * factor, m_scale.y * factor, m_scale.z * factor });
		}

		void rotate(const qf64& q) noexcept
		{
			set_rotation(m_rotation * q);
		}


	private:

        mutable m4f64 m_transformations{ 1 };

		v3f64		m_scale{};
		v3f64		m_origin{};
		types::pos	m_position{};

		qf64		m_rotation{ 1., 0., 0., 0. };

		mutable bool		m_transformNeedUpdate{ false };


	};


} // global