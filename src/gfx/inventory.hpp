#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	The player's storage; all items that are stored within the inventory
* ==============================================-
*/

#include <array>
#include <vector>

#include "sys/types.hpp"

#include "items.hpp"


namespace gfx
{

	class Inventory
	{
	public:

		struct Stage
		{
			// In Slots
			v2f32 size{};
			u16 count_per_slot{};
			u8 size_hotbar{};
		};

		enum Size : u8
		{
			Small,
			Medium,
			Big
		};


	public:

		Inventory()
			: m_size{ Small }
		{
			m_item_stacks.resize(get_nb_slots().x * get_nb_slots().y);
			m_item_stacks_hb.resize(get_nb_slots_hb());

			set_item_stack(0, ItemStack{ { 1, {} }, g_stages[m_size].count_per_slot, 1 });
			set_item_stack(1, ItemStack{ { 1, {} }, g_stages[m_size].count_per_slot, 10 }); 
			set_item_stack(2, ItemStack{ { 1, {} }, g_stages[m_size].count_per_slot, 99 }); 
			set_item_stack(3, ItemStack{ { 1, {} }, g_stages[m_size].count_per_slot, 20 }); 
			set_item_stack(4, ItemStack{ { 1, {} }, g_stages[m_size].count_per_slot, 90 }); 

			set_item_stack_hb(0, ItemStack{ { 1, {} }, g_stages[m_size].count_per_slot, 1 });
			set_item_stack_hb(1, ItemStack{ { 1, {} }, g_stages[m_size].count_per_slot, 10 });
			set_item_stack_hb(2, ItemStack{ { 1, {} }, g_stages[m_size].count_per_slot, 99 });
		}

		/// <summary>
		/// Sets the stage pointer to another stage; therefore, the stage passed in must be one of the three predefined ones
		/// </summary>
		/// <param name="stage"></param>
		void set_stage(Size stage) noexcept;

		u8 get_change() const noexcept { return m_change; }

		Size get_size() const noexcept { return m_size; }

		v2f32 get_nb_slots() const noexcept { return g_stages[m_size].size; }

		u16 get_nb_slots_hb() const noexcept { return g_stages[m_size].size_hotbar; }

		std::optional<ItemStack> get_item_stack(size_t i) const noexcept
		{
			if (i < 0 || i >= m_item_stacks.size())
				return std::nullopt;

			return std::make_optional(m_item_stacks.at(i));
		}

		std::optional<ItemStack> get_item_stack_hb(size_t i) const noexcept
		{
			if (i < 0 || i >= m_item_stacks_hb.size())
				return std::nullopt;

			return std::make_optional(m_item_stacks_hb.at(i));
		}

		ItemStack get_temp() const noexcept { return m_temp; }


		void set_item_stack(size_t index, ItemStack item_stack) noexcept 
		{
			m_change++;
			m_item_stacks.at(index).set(item_stack.get_type(), g_stages[m_size].count_per_slot, item_stack.count());
		}

		void set_item_stack_hb(size_t index, ItemStack item_stack) noexcept
		{
			m_change++;
			m_item_stacks_hb.at(index).set(item_stack.get_type(), g_stages[m_size].count_per_slot, item_stack.count());
		}

		void set_temp(ItemStack item_stack) noexcept
		{
			m_temp.set(item_stack);
		}


	public:

		static constexpr std::array<Stage, 3> g_stages
		{
			Stage
			{ .size{ 6 , 4 }, .count_per_slot{ 100 }, .size_hotbar{ 6 } },
			{ .size{ 8 , 5 }, .count_per_slot{ 100 }, .size_hotbar{ 8 } },
			{ .size{ 12, 6 }, .count_per_slot{ 100 }, .size_hotbar{ 12 } }
		};


	private:

		Size m_size;

		u8 m_change{ 1 };

		std::vector<ItemStack> m_item_stacks;
		std::vector<ItemStack> m_item_stacks_hb;

		ItemStack m_temp;


	};


}