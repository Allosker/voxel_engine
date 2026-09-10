#include "inventory.hpp"

namespace gfx
{

	void Inventory::set_stage(Size size) noexcept
	{
		m_size = size;
		const auto& new_stage = g_stages[size];
		m_item_stacks.resize(new_stage.size.x * new_stage.size.y);
	}

	u32 Inventory::add_items(ItemStack::Type type, u32 count) noexcept
	{
		bool hotbar_full{ false };
		size_t i{};
		while (count)
		{
			if (!hotbar_full)
			{
				if (i >= m_item_stacks_hb.size())
				{
					i = 0;
					hotbar_full = true;
					continue;
				}

				count = m_item_stacks_hb.at(i).add(type, count);

				i++;
				continue;
			}

			if (i >= m_item_stacks.size()) break;

			count = m_item_stacks.at(i).add(type, count);

			i++;
		} 

		m_change++;
		return count;
	}

	u32 Inventory::remove_items(ItemStack::Type type, u32 count) noexcept
	{
		m_change++;
		return {};
	}

	void Inventory::clear() noexcept
	{
		m_item_stacks.clear();
		m_item_stacks.resize(get_nb_slots().x * get_nb_slots().y);

		m_item_stacks_hb.clear();
		m_item_stacks_hb.resize(get_nb_slots_hb());
	}




}