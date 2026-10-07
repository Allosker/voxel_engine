#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	The inventory regroups all that is inventory-related; such as: the inventory, the crafting system...
* ==============================================-
*/

#include "gfx/inventory.hpp"
#include "sys/window.hpp"
#include <sys/types.hpp>


namespace gfx
{

	class PlayerInventory
	{
	public:

		enum State : u8
		{
			Inventory,
			Crafting,
			Armor
		};


		/// <returns>whether the mouse cursor should be toggled or not</returns>
		void toggle(Window& window) noexcept
		{
			m_inv.toggle();

			m_is_active = !m_is_active;

			if (m_is_active)
			{
				m_was_cursor_visible = window.is_cursor_visible();
				window.set_cursor_sight(true);
			}
			else
			{
				if (!m_was_cursor_visible)
					window.set_cursor_sight(false);
			}

		}

		bool is_active() const noexcept { return m_is_active; }

		gfx::Inventory& get_inventory() noexcept { return m_inv; }

		void set_state(State state) noexcept
		{
			m_state = state;

			if (state == State::Inventory)
			{
				m_inv.set_active(true);
			}
			else
				m_inv.set_active(false);

		}

		State get_state() const noexcept { return m_state; }


	private:

		gfx::Inventory		m_inv;


		State m_state{};

		bool m_is_active{};
		bool m_was_cursor_visible{}; // when the inventory was toggled on

	};

}

