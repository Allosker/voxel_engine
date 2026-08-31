#include "gui/inventoryGUI.hpp"

#include <mpml/vectors/special_overloads/print_vectors.hpp>
#include <print>

namespace gui
{


	void InventoryGUI::update(const gfx::Inventory& inv, types::pos2d gui_mouse_pos) noexcept
	{
		if (inv.get_size() != m_size)
		{
			m_size = inv.get_size();
			change_inventory(m_size);

			m_nb_slots = inv.get_nb_slots();
			m_rec.get_hitbox().set_extent(m_rec.get_texture()->get_size());
		}

		compute_index(gui_mouse_pos);
	}


	void InventoryGUI::change_inventory(gfx::Inventory::Size size) noexcept
	{
		switch (size)   
		{
			case gfx::Inventory::Size::Small:
				m_rec.update_sprite(&AssetsManager::get().textures.at("textures/gui/inventory/small"));
				break;

			case gfx::Inventory::Size::Medium:
				m_rec.update_sprite(&AssetsManager::get().textures.at("textures/gui/inventory/medium"));
				break;

			case gfx::Inventory::Size::Big:
				m_rec.update_sprite(&AssetsManager::get().textures.at("textures/gui/inventory/big"));
				break;
				 
			default:
				break;
		}
	}

	void InventoryGUI::compute_index(types::pos2d gui_mouse_pos) noexcept
	{
		phy::HitboxAABB2D board{ m_rec.transform().get_pos(), m_rec.transform().get_size() - g_outline };


		if (phy::intersects(board, gui_mouse_pos))
		{
			v2f32 converted_mp{ gui_mouse_pos - board.get_min()};
			v2u64 index2d{ static_cast<size_t>(converted_mp.x / g_slot_size), static_cast<size_t>(converted_mp.y / g_slot_size) };
			
			m_index = std::make_optional(index2d.x + index2d.y * m_nb_slots.x);
		}
		else
			m_index = std::nullopt;
	}


}