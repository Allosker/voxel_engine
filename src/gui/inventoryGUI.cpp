#include "gui/inventoryGUI.hpp"

#include "sys/window.hpp"

#include <print>
#include "gfx/debugRenderer.hpp"

namespace gui
{


	void InventoryGUI::update(types::pos2d gui_mouse_pos) noexcept
	{
		if (m_inv_change != m_inv.get_change())
			update_items();


		compute_index(gui_mouse_pos);


		if (m_index)
		{
			m_item_stacks.at(*m_index).set_scale(g_over_ISG_scale);

			if (m_last_index)
				if (*m_last_index != *m_index)
				{
					m_item_stacks.at(*m_last_index).set_scale(g_base_ISG_scale);
				}
		}
		else
		{
			if (m_last_index)
				m_item_stacks.at(*m_last_index).set_scale(g_base_ISG_scale);
		}
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

	void InventoryGUI::update_items() noexcept
	{
		change_board(m_inv.get_size());

		m_nb_slots = m_inv.get_nb_slots();
		m_board.get_hitbox().set_extent(m_board.get_texture()->get_size());

		m_board.set_pos({ Window::g_gui_view_size.x / 2, Window::g_gui_view_size.y / 2 });

		types::pos2d start_pos{ m_board.get_pos() - m_board.get_size() + g_outline };
		types::pos2d slot_pos{ start_pos };
		for (i32 y{}; y < m_inv.get_nb_slots().y; y++)
		{
			slot_pos.x = start_pos.x;
			for (i32 x{}; x < m_inv.get_nb_slots().x; x++)
			{
				const auto& i = m_inv.get_item_stack(x + y * m_inv.get_nb_slots().x);
				if (i && i->get_type().id != types::TypeIdNull)
				{ // Make it so that you only emplace everything once
					m_item_stacks.emplace_back();
					m_item_stacks.back().set_scale(g_base_ISG_scale);
					m_item_stacks.back().set_pos(v3f32{ slot_pos, 0. } + g_slot_size / 2.f);
					m_item_stacks.back().rotate(glm::angleAxis<f32>(glm::radians(70.f), glm::normalize(v3f32{ 1, 0, 0 })));
					m_item_stacks.back().rotate(glm::angleAxis<f32>(glm::radians(45.f), glm::normalize(v3f32{ 0, 0, 1 })));

					// When there are item models, change it so that it can accept either of them
					m_item_stacks.back().update_model(i->get_type().id);
				}

				slot_pos.x += g_slot_size;
			}

			slot_pos.y += g_slot_size;
		}
	}

	void InventoryGUI::compute_index(types::pos2d gui_mouse_pos) noexcept
	{
		phy::HitboxAABB2D board{ m_board.get_pos(), m_board.get_size() - g_outline };

		if (phy::intersects(board, gui_mouse_pos))
		{
			v2f32 converted_mp{ gui_mouse_pos - board.get_min()};
			v2u64 index2d{ static_cast<size_t>(converted_mp.x / (g_slot_size + 1)), static_cast<size_t>(converted_mp.y / (g_slot_size + 1)) };
			
			m_last_index = m_index;
			m_index = std::make_optional(index2d.x + index2d.y * m_nb_slots.x);
		}
		else
			m_index = std::nullopt;
	}


}