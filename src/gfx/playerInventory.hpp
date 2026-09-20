#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	The inventory regroups all that is inventory-related; such as: the inventory, the crafting system...
* ==============================================-
*/

#include "gfx/inventory.hpp"
#include "gui/inventoryGUI.hpp"
#include "sys/window.hpp"


namespace gfx
{

	class PlayerInventory
	{
	public:


		/// <returns>whether the mouse cursor should be toggled or not</returns>
		void toggle(Window& window) noexcept
		{
			m_inv.toggle();

			is_active = !is_active;

			if (is_active)
			{
				was_cursor_visible = window.is_cursor_visible();
				window.set_cursor_sight(true);
			}
			else
			{
				if (!was_cursor_visible)
					window.set_cursor_sight(false);
			}
			
		}

		gfx::Inventory& get_inventory() noexcept { return m_inv; }


	private:


		gfx::Inventory		m_inv;

		bool is_active{};
		bool was_cursor_visible{}; // when the inventory was toggled on

	};

}

