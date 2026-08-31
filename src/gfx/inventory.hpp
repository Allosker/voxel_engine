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

		enum Size : i8
		{
			None = -1, // This class' Size var can never have this value
			Small,
			Medium,
			Big
		};


	public:

		Inventory()
			: m_size{ Small }
		{
		}

		/// <summary>
		/// Sets the stage pointer to another stage; therefore, the stage passed in must be one of the three predefined ones
		/// </summary>
		/// <param name="stage"></param>
		void set_stage(Size stage) noexcept;

		Size get_size() const noexcept { return m_size; }

		v2f32 get_nb_slots() const noexcept { return g_stages[m_size].size; }



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

		std::vector<ItemStack> m_item_stacks;

	};


}