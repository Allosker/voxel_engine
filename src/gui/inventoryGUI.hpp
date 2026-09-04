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
#include "sys/inputManager.hpp"


namespace gui
{

	class InventoryGUI
	{
	public:

		InventoryGUI(gfx::Inventory& inv)
			:m_inv{ inv }, m_board{ {} }
		{  
			m_board.set_scale(g_scale);
			m_dh_click = sys::InputManager::get().subscribe(&InventoryGUI::on_click, *this, Event::MouseButtonEvent{ .scancode{} });

			m_temp.set_scale(g_over_ISG_scale);
			m_temp.set_pos(v3f32{ 0., 0., 0. } + g_slot_size / 2.f);
			m_temp.rotate(glm::angleAxis<f32>(glm::radians(70.f), glm::normalize(v3f32{ 1, 0, 0 })));
			m_temp.rotate(glm::angleAxis<f32>(glm::radians(45.f), glm::normalize(v3f32{ 0, 0, 1 })));
		}

		~InventoryGUI()
		{
			sys::InputManager::get().unsubscribe(m_dh_click);
		}

		/// <summary>
		/// Update the GUI according to the 
		/// </summary>
		/// <param name="gui_mouse_pos"></param>
		void update(types::pos2d gui_mouse_pos) noexcept;

		std::optional<size_t> get_slot_index() const noexcept { return m_index; }

		
		void on_click(Event::MouseButtonEvent event) noexcept;


		void draw(const gfx::RenderContext& inv_c, const gfx::RenderContext& is_c) noexcept
		{
			m_board.draw(inv_c);


			glEnable(GL_DEPTH_TEST);

			is_c.sha->bind();

			for (auto& i : m_item_stacks)
				i.draw(is_c);

			m_temp.draw(is_c);

			is_c.sha->unbind();
		}



	private:

		void change_board(gfx::Inventory::Size size) noexcept;

		void update_items() noexcept;

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

		ItemStackGUI m_temp;
		gfx::Rectangle m_board;
		std::vector<ItemStackGUI> m_item_stacks;

		sys::InputManager::DelegateHandle m_dh_click{};

		gfx::Inventory& m_inv;
		v2f32 m_nb_slots{};

		std::optional<size_t> m_index{};
		std::optional<size_t> m_last_index{};

		bool m_picked_item_up{};
		bool m_move_temp_to_mouse{};
		u8 m_inv_change{};


	};

}