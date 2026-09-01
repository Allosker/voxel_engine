#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Graphical representation of the Inventory of the player.
* ==============================================-
*/

#include <optional>
#include <vector>

#include "gfx/inventory.hpp"
#include "gfx/rectangle.hpp"
#include "gfx/renderContext.hpp"
#include "sys/assetsManager.hpp"

#include "gui/itemStackGUI.hpp"


namespace gui
{

	class InventoryGUI
	{
	public:

		InventoryGUI()
			: m_board{ {} }
		{  
			m_board.set_scale(g_scale);
		}


		void update( gfx::Inventory& inv, types::pos2d gui_mouse_pos) noexcept;

		std::optional<size_t> get_slot_index() const noexcept { return m_index; }


		void draw(const gfx::RenderContext& inv_c, const gfx::RenderContext& is_c) noexcept
		{
			m_board.draw(inv_c);


			glEnable(GL_DEPTH_TEST);

			is_c.sha->bind();

			for (auto& i : m_item_stacks)
				i.draw(is_c);

			is_c.sha->unbind();
		}



	private:

		void change_board(gfx::Inventory::Size size) noexcept;

		void update_items(const gfx::Inventory& inv) noexcept;

		void compute_index(types::pos2d gui_mouse_pos) noexcept;

		/// <summary>
		/// Set of constants based on the following files:
		///		textures/gui/inventory/small.png
		///		textures/gui/inventory/medium.png
		///		textures/gui/inventory/big.png
		/// </summary>
		static constexpr f32 g_outline_thickness_px{ 13.f };
		static constexpr float c_absolute_slot_size_px{ 32.f };
		static constexpr f32 g_scale{ 1.5f };
		static constexpr f32 g_outline{ g_outline_thickness_px * g_scale * 2.f };
		static constexpr f32 g_slot_size{ c_absolute_slot_size_px * g_scale * 2.f };
		
		static constexpr f32 g_base_ISG_scale{ g_slot_size / 2.5f };
		static constexpr f32 g_over_ISG_scale{ g_slot_size / 2.f };

		/// <summary>
		/// Changes according to the inventory's stage
		/// </summary>
		v2f32 m_slot_size{};


	private:

		gfx::Rectangle m_board;

		std::vector<ItemStackGUI> m_item_stacks;

		
		v2f32 m_nb_slots{};

		std::optional<size_t> m_index{};
		std::optional<size_t> m_last_index{};

		u8 m_inv_change{};
	};

}