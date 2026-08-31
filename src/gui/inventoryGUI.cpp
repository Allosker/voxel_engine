#include "gui/inventoryGUI.hpp"

#include "sys/window.hpp"


namespace gui
{


	void InventoryGUI::update(const gfx::Inventory& inv, types::pos2d gui_mouse_pos) noexcept
	{
		if (inv.get_size() != m_size)
		{
			m_size = inv.get_size();
			change_board(m_size);

			m_nb_slots = inv.get_nb_slots();
			m_board.get_hitbox().set_extent(m_board.get_texture()->get_size());

			m_board.set_pos({ Window::g_gui_view_size.x / 2, Window::g_gui_view_size.y / 2 });

			types::pos2d start_pos{ m_board.get_pos() - m_board.get_size() + g_outline };
			types::pos2d slot_pos{ start_pos };
			for (i32 y{}; y < inv.get_nb_slots().y; y++)
			{
				slot_pos.x = start_pos.x;
				for (i32 x{}; x < inv.get_nb_slots().x; x++)
				{
					m_item_stacks.emplace_back();
					m_item_stacks.back().set_scale(g_slot_size / 2.f);
					m_item_stacks.back().set_pos(v3f32{ slot_pos, 0.} + g_slot_size / 2.f);
					m_item_stacks.back().rotate(glm::angleAxis<f32>(glm::radians(70.f), glm::normalize(v3f32{ 1, 0, 0 })));
					m_item_stacks.back().rotate(glm::angleAxis<f32>(glm::radians(45.f), glm::normalize(v3f32{ 0, 0, 1 })));
					
					slot_pos.x += g_slot_size;
				}

				slot_pos.y += g_slot_size;
			}
		}

		compute_index(gui_mouse_pos);

		if (m_index)
		{
			/*m_item_stacks.at(*m_index).scale(1.5f);*/
		}

		/*if (m_last_index)
		{
			if (!m_index || (m_index && *m_last_index != *m_index))
				m_item_stacks.at(*m_last_index).scale(0.5f);
		}*/
	}


	void InventoryGUI::change_board(gfx::Inventory::Size size) noexcept
	{
		switch (size)   
		{
			case gfx::Inventory::Size::Small:
				m_board.update_sprite(&AssetsManager::get().textures.at("textures/gui/inventory/small"));
				break;

			case gfx::Inventory::Size::Medium:
				m_board.update_sprite(&AssetsManager::get().textures.at("textures/gui/inventory/medium"));
				break;

			case gfx::Inventory::Size::Big:
				m_board.update_sprite(&AssetsManager::get().textures.at("textures/gui/inventory/big"));
				break;
				 
			default:
				break;
		}
	}

	void InventoryGUI::compute_index(types::pos2d gui_mouse_pos) noexcept
	{
		phy::HitboxAABB2D board{ m_board.get_pos() + m_board.get_size(), m_board.get_size() - g_outline};


		if (phy::intersects(board, gui_mouse_pos))
		{
			v2f32 converted_mp{ gui_mouse_pos - board.get_min()};
			v2u64 index2d{ static_cast<size_t>(converted_mp.x / g_slot_size), static_cast<size_t>(converted_mp.y / g_slot_size) };
			
			m_last_index = m_index;
			m_index = std::make_optional(index2d.x + index2d.y * m_nb_slots.x);
		}
		else
			m_index = std::nullopt;
	}


}