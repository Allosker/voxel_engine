#include "items.hpp"

namespace gfx
{
	u32 ItemStack::add(Type type, u32 count) noexcept
	{
		if (m_type == types::type_id_null)
			m_type = type;
		else if (m_type != type) return 0;

		u32 remainder{};

		if (count + m_count > m_max_count)
		{
			u32 cast_count{ static_cast<u32>(m_count) };
			remainder = count + cast_count - m_max_count;

			m_count = m_max_count;
		}
		else
		{
			m_count += count;
		}

		return remainder;
	}

	u32 ItemStack::take(Type type, u32 count) noexcept
	{
		if (type != m_type) return 0;

		if (count >= m_count)
			return take_all();

		m_count -= count;

		return count;
	}

}