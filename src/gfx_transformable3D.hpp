#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	3D transformable that creates a 4x4 model matrix as a result of any translation/scaling/rotation it undergoes
* Mainly for any game object that needs transformations
* ==============================================-
*/

#include "sys_types.hpp"


namespace gfx
{

	class Transformable3D
	{
	public:

		Transformable3D(const v3f64& size = {}, const types::pos& ori = {})
			: m_origin{ ori }, m_baseSize{ size }, m_scale{ 1.f, 1.f, 1.f }
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

		const m4f64& get_transform() noexcept
		{
			if (m_transformNeedUpdate)
			{
				m4f64 transforms{ m4f64::Identity };

				transforms = mpml::scale(transforms, m_scale);
				transforms = mpml::rotate(transforms, m_rotation);
				transforms = mpml::translate(transforms, m_position);

				m_transformations = transforms;

				m_transformNeedUpdate = false;
			}

			return m_transformations;
		}

		const v3f64& get_size() const noexcept { return { m_baseSize.x * m_scale.x, m_baseSize.y * m_scale.y, m_baseSize.z * m_scale.z }; }

		qf64 get_rotation() const noexcept { return m_rotation; }

		const types::pos& get_pos() const noexcept { return m_position; }

		const v3f64& get_baseSize() const noexcept { return m_baseSize; }


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

		void set_size(const v3f64& size) noexcept
		{
			if (m_baseSize != 0)
				set_scale({ size.x / m_baseSize.x, size.y / m_baseSize.y, size.z / m_baseSize.z });
			else
				m_baseSize = size;

			m_transformNeedUpdate = true;
		}

		void set_baseSize(const v3f64& size) noexcept
		{
			m_baseSize = size;
			m_transformNeedUpdate = true;
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

		void rotate(const qf64& q) noexcept
		{
			set_rotation(m_rotation * q);
		}


	private:

		m4f64		m_transformations{ m4f64::Identity };

		v3f64		m_scale{};
		v3f64		m_baseSize{};
		v3f64		m_origin{};
		types::pos	m_position{};

		qf64		m_rotation{ 1., 0, 0, 0 };

		bool		m_transformNeedUpdate{ false };


	};


} // global