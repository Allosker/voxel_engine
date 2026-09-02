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
			set_item_stack(0, 1);
		}

		/// <summary>
		/// Sets the stage pointer to another stage; therefore, the stage passed in must be one of the three predefined ones
		/// </summary>
		/// <param name="stage"></param>
		void set_stage(Size stage) noexcept;

		u8 get_change() const noexcept { return m_change; }

		Size get_size() const noexcept { return m_size; }

		v2f32 get_nb_slots() const noexcept { return g_stages[m_size].size; }

		std::optional<ItemStack> get_item_stack(size_t i) const noexcept
		{
			if (i < 0 || i >= m_item_stacks.size())
				return std::nullopt;

			return std::make_optional(m_item_stacks.at(i));
		}

		void set_item_stack(size_t index, types::type_id id) noexcept 
		{
			m_change++;
			m_item_stacks.at(index).set({ id, {} }, 10, 10);
		}


	public:

		static constexpr std::array<Stage, 3> g_stages
		{
			Stage
			{ .size{ 6 , 4 }, .count_per_slot{ 100 } },
			{ .size{ 8 , 5 }, .count_per_slot{ 100 } },
			{ .size{ 12, 6 }, .count_per_slot{ 100 } }
		};


	private:

		Size m_size;

		u8 m_change{ 1 };

		std::vector<ItemStack> m_item_stacks;

	};


}