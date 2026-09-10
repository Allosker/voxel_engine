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

		size_t get_index_hb() const noexcept { return m_index_hb; }


		void toggle() noexcept { m_active = !m_active; m_change++; }

		/// <summary>
		/// Tries to add the Item.s
		/// <para>Note: count can be any number in I</para> 
		/// </summary>
		/// <param name="type"></param>
		/// <param name="count"></param>
		/// <returns>The count of items that could not be added</returns> 
		[[nodiscard]] u32 add_items(ItemStack::Type type, u32 count) noexcept;

		/// <summary>
		/// Tries to remove the Item.s, returns how many could be removed 
		/// <para>If none could, returns 0</para>
		/// <para>Note: count can be any number in I</para> 
		/// </summary>
		/// <param name="type"></param>
		/// <param name="count"></param>
		/// <returns>The count of removed item</returns>
		[[nodiscard]] u32 remove_items(ItemStack::Type type, u32 count) noexcept;

		void clear() noexcept;


		bool is_active() const noexcept { return m_active; }


		void on_mouse_scroll(v2f32 delta) noexcept
		{
			if (m_active) return;

			m_index_hb = ((m_index_hb + static_cast<i64>(delta.y)) % get_nb_slots_hb() + get_nb_slots_hb()) % get_nb_slots_hb();

			m_change++;
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

		std::vector<ItemStack> m_item_stacks;
		std::vector<ItemStack> m_item_stacks_hb;
		i64 m_index_hb{};

		ItemStack m_temp;

		Size m_size;

		u8 m_change{ 1 };
		bool m_active{};


	};


}