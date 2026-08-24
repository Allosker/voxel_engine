#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	The player's storage; all items that are stored within the inventory
* ==============================================-
*/

#include <vector>

#include "sys/types.hpp"

#include "items.hpp"


namespace gfx
{

	class Inventory
	{
		struct Stage
		{
			// In Slots
			v3i16 size{};
			u16 count_per_slot{};
		};

	public:

		Inventory()
			: m_stage{ &small }
		{
		}

		/// <summary>
		/// Sets the stage pointer to another stage; therefore, the stage passed in must be one of the three predefined ones
		/// </summary>
		/// <param name="stage"></param>
		void set_stage(const Stage& stage) noexcept;


	public:

		constexpr static Stage small	{ .size{ 6 , 4 }, .count_per_slot{ 100 } };
		constexpr static Stage medium	{ .size{ 8 , 5 }, .count_per_slot{ 100 } };
		constexpr static Stage big		{ .size{ 12, 6 }, .count_per_slot{ 100 } };


	private:

		const Stage* m_stage;

		std::vector<ItemStack> m_item_stacks;

	};


}