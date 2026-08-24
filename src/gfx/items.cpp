#include "items.hpp"

namespace gfx
{
	u16 ItemStack::add(Type type, u16 count) noexcept
	{
		if (type != m_type) return 0;

		u16 remainder{};

		if (count + m_count > m_max_count)
		{
			remainder = count + m_count - m_max_count;

			m_count = m_max_count;
		}
		else
		{
			m_count += count;
		}

		return remainder;
	}

	u16 ItemStack::take(Type type, u16 count) noexcept
	{
		if (type != m_type) return 0;

		if (count >= m_count)
			return take_all();

		m_count -= count;

		return count;
	}

}