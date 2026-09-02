#include "gui/inventoryGUI.hpp"

#include "sys/window.hpp"
#include <sys/inputTypes.hpp>


namespace gui
{


	void InventoryGUI::update(types::pos2d gui_mouse_pos) noexcept
	{
		if (m_inv_change != m_inv.get_change())
		{
			update_items();
			m_inv_change = m_inv.get_change();
		}

		if (m_move_temp_to_mouse)
			m_temp.set_pos({ gui_mouse_pos, 0. });

		compute_index(gui_mouse_pos);


		// Highlight to know on which slot the mouse is
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

	void InventoryGUI::on_click(Event::MouseButtonEvent event) noexcept
	{
		if (event.scancode == MouseButtons::Left)
		{
			if (event.state == Event::ButtonState::Press && !m_picked_item_up)
			{
				if (!m_index) return;

				const auto current = m_inv.get_item_stack(*m_index);

				if (!current->get_type()) return;


				m_inv.set_temp(*current);
				m_inv.set_item_stack(*m_index, {});  


				// GUI

				m_temp.update_model(m_inv.get_temp().get_type().id);
				m_temp.set_should_be_drawn(true);
				m_move_temp_to_mouse = true;

				m_item_stacks.at(*m_index).set_should_be_drawn(false);

				m_picked_item_up = true;
			}
			else if (event.state == Event::ButtonState::Press && m_picked_item_up)
			{
				if (!m_index) return;

				const auto current = m_inv.get_item_stack(*m_index);

				if (current->get_type() && current->get_type() != m_inv.get_temp().get_type()) return;


				gfx::ItemStack is = *current;

				is.set(m_inv.get_temp());

				m_inv.set_item_stack(*m_index, is);

				m_inv.set_temp({});


				// GUI

				m_temp.set_should_be_drawn(false);
				m_move_temp_to_mouse = false;

				m_item_stacks.at(*m_index).set_should_be_drawn(true);

				m_picked_item_up = false;
			}
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

		const auto target_size_inv = m_inv.get_nb_slots().x * m_inv.get_nb_slots().y;

		types::pos2d start_pos{ m_board.get_pos() - m_board.get_size() + g_outline };
		types::pos2d slot_pos{ start_pos };
		for (i32 y{}; y < m_inv.get_nb_slots().y; y++)
		{
			slot_pos.x = start_pos.x;
			for (i32 x{}; x < m_inv.get_nb_slots().x; x++)
			{
				ItemStackGUI* isg = nullptr;

				if (m_item_stacks.size() < target_size_inv)
				{
					m_item_stacks.emplace_back();
					isg = &m_item_stacks.back();

					isg->set_scale(g_base_ISG_scale);
					isg->set_pos(v3f32{ slot_pos, -100. } + g_slot_size / 2.f);
					isg->rotate(glm::angleAxis<f32>(glm::radians(70.f), glm::normalize(v3f32{ 1, 0, 0 })));
					isg->rotate(glm::angleAxis<f32>(glm::radians(45.f), glm::normalize(v3f32{ 0, 0, 1 })));
				}
				else
				{
					isg = &m_item_stacks.at(x + y * m_inv.get_nb_slots().x);
				}

				const auto& i = m_inv.get_item_stack(x + y * m_inv.get_nb_slots().x);
				if (i && i->get_type().id != types::TypeIdNull)
				{
					// When there are item models, change it so that it can accept either of them
					isg->update_model(i->get_type().id);
					isg->set_should_be_drawn(true);
				}
				else
					isg->set_should_be_drawn(false);


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
			v2f32 converted_mp{ gui_mouse_pos - board.get_min() };
			v2u64 index2d{ static_cast<size_t>(converted_mp.x / (g_slot_size + 1)), static_cast<size_t>(converted_mp.y / (g_slot_size + 1)) };

			m_last_index = m_index;
			m_index = std::make_optional(index2d.x + index2d.y * m_nb_slots.x);
		}
		else
			m_index = std::nullopt;
	}


}