#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	The inventory regroups all that is inventory-related; such as: the inventory, the crafting system...
* ==============================================-
*/

#include "gfx/inventory.hpp"
#include "gui/inventoryGUI.hpp"

namespace gfx
{

	class PlayerInventory
	{
	public:


		gfx::Inventory& get_inventory() noexcept { return m_inv; }


	private:


		gfx::Inventory		m_inv;

	};

}

