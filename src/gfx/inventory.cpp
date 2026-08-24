#include "inventory.hpp"

namespace gfx
{



	void Inventory::set_stage(const Stage& stage) noexcept
	{
		m_stage = &stage;
		m_item_stacks.resize(stage.size.x * stage.size.y);
	}



}