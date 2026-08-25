#include "inventory.hpp"

namespace gfx
{



	void Inventory::set_stage(Size size) noexcept
	{
		m_size = size;
		const auto& new_stage = g_stages[size];
		m_item_stacks.resize(new_stage.size.x * new_stage.size.y);
	}



}