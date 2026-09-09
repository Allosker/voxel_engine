#include "gui/inventoryGUI.hpp"

#include "sys/window.hpp"
#include <string>
#include <sys/inputTypes.hpp>


namespace gui
{


	void InventoryGUI::update(types::pos2d gui_mouse_pos) noexcept
	{
		if (m_inv_change != m_inv.get_change())
		{
			update_items();

			m_selected_slot.set_pos({
				Window::g_gui_view_size.x / 2 - m_hotbar.get_size().x + g_outline_hb + m_selected_slot.get_size().y, 
				Window::g_gui_view_size.y - m_selected_slot.get_size().y
			}); // Position the selected slot right on the first hotbar slot
			m_selected_slot.move({ g_slot_size * m_inv.get_index_hb(), {} });

			m_inv_change = m_inv.get_change();
		}

		if (m_inv.is_active() && m_move_temp_to_mouse)
			m_temp.set_pos({ gui_mouse_pos, 0. });

		// Highlight to know on which slot the mouse is
		const auto highlight = [&](auto& stacks, const auto& new_index, auto& index)
			{
				if (new_index)
				{
					auto& current = stacks.at(*new_index);
					current.set_scale_text(g_over_ISG_text_scale);
					current.set_scale(g_over_ISG_scale);

					if (index)
						if (new_index != index)
						{
							auto& current = stacks.at(*index);
							current.set_scale_text(g_base_ISG_text_scale);
							current.set_scale(g_base_ISG_scale);
						}

					index = new_index;
				}
				else if (index)
				{
					auto& current = stacks.at(*index);
					current.set_scale_text(g_base_ISG_text_scale);
					current.set_scale(g_base_ISG_scale);

					index = new_index;
				}
			};

		if (m_inv.is_active())
			highlight(m_item_stacks, compute_index(gui_mouse_pos), m_index);
		highlight(m_item_stacks_hb, compute_index_hb(gui_mouse_pos), m_index_hb);

	}

	void InventoryGUI::on_click(Event::MouseButtonEvent event) noexcept
	{
		if (event.scancode == MouseButtons::Left)
		{
			if (event.state == Event::ButtonState::Press && !m_picked_item_up)
			{
				if (m_inv.is_active() && m_index)
				{
					const auto current = m_inv.get_item_stack(*m_index);

					if (!current->get_type()) return;

					m_inv.set_temp(*current);
					m_inv.set_item_stack(*m_index, {});

					m_item_stacks.at(*m_index).set_should_be_drawn(false);
				}
				else if (m_index_hb)
				{
					const auto current = m_inv.get_item_stack_hb(*m_index_hb);

					if (!current->get_type()) return;

					m_inv.set_temp(*current);
					m_inv.set_item_stack_hb(*m_index_hb, {});

					m_item_stacks_hb.at(*m_index_hb).set_should_be_drawn(false);
				}
				else return;

				// GUI

				m_temp.update(m_inv.get_temp().get_type(), std::to_string(m_inv.get_temp().count()));
				m_temp.set_should_be_drawn(true);
				m_move_temp_to_mouse = true;

				m_picked_item_up = true;
			}
			else if (event.state == Event::ButtonState::Press && m_picked_item_up)
			{
				bool is_there_leftover{};

				if (m_inv.is_active() && m_index)
				{
					const auto current = m_inv.get_item_stack(*m_index);

					if (current->get_type() && current->get_type() != m_inv.get_temp().get_type()) return;


					gfx::ItemStack is = *current;


					if (!current->get_type() || !current->count())
					{
						is.set(m_inv.get_temp());

						m_inv.set_item_stack(*m_index, is);

						m_inv.set_temp({});
					}
					else
					{
						const auto remainder = is.add(m_inv.get_temp().get_type(), m_inv.get_temp().count());

						m_inv.set_item_stack(*m_index, is);

						m_inv.set_temp({ is.get_type(), is.max_count(), remainder });
						is_there_leftover = remainder;
					}

					m_item_stacks.at(*m_index).set_should_be_drawn(true);
				}
				else if (m_index_hb)
				{
					const auto current = m_inv.get_item_stack_hb(*m_index_hb);

					if (current->get_type() && current->get_type() != m_inv.get_temp().get_type()) return;


					gfx::ItemStack is = *current;


					if (!current->get_type() || !current->count())
					{
						is.set(m_inv.get_temp());

						m_inv.set_item_stack_hb(*m_index_hb, is);

						m_inv.set_temp({});
					}
					else
					{
						const auto remainder = is.add(m_inv.get_temp().get_type(), m_inv.get_temp().count());

						m_inv.set_item_stack_hb(*m_index_hb, is);

						m_inv.set_temp({ is.get_type(), is.max_count(), remainder });
						is_there_leftover = remainder;
					}

					m_item_stacks_hb.at(*m_index_hb).set_should_be_drawn(true);
				}
				else return;


				// GUI  

				m_temp.set_should_be_drawn(is_there_leftover);
				m_move_temp_to_mouse = is_there_leftover;
				if (is_there_leftover)
					m_temp.update(m_inv.get_temp().get_type(), std::to_string(m_inv.get_temp().count()));

				m_picked_item_up = is_there_leftover;
			}
		}
	}


	void InventoryGUI::change_textures(gfx::Inventory::Size size) noexcept
	{
		switch (size)
		{
			case gfx::Inventory::Size::Small:
				m_board.update_sprite(&AssetsManager::get().textures.at("textures/gui/inventory/small"));
				m_hotbar.update_sprite(&AssetsManager::get().textures.at("textures/gui/inventory/hotbar_small"));
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
		const auto update_item_stack = [&](std::vector<ItemStackGUI>& stacks, const auto target_size, const auto current_index, const auto& is, const auto& slot_pos) 
			{
				ItemStackGUI* isg = nullptr;

				if (stacks.size() < target_size)
				{
					stacks.emplace_back();
					isg = &stacks.back();

					isg->set_scale(g_base_ISG_scale);
					isg->set_pos(v3f32{ slot_pos, -100. } + g_slot_size / 2.f);
					isg->rotate(glm::angleAxis<f32>(glm::radians(70.f), glm::normalize(v3f32{ 1, 0, 0 })));
					isg->rotate(glm::angleAxis<f32>(glm::radians(45.f), glm::normalize(v3f32{ 0, 0, 1 })));
					isg->set_scale_text(g_base_ISG_text_scale);
				}
				else
				{
					isg = &stacks.at(current_index);
				}

				if (is && is->get_type().id != types::TypeIdNull)
				{
					// When there are item models, change it so that it can accept either of them
					isg->update(is->get_type(), std::to_string(is->count()));
					isg->set_text_pos();
					isg->set_should_be_drawn(true);
				}
				else
					isg->set_should_be_drawn(false);
			};

		change_textures(m_inv.get_size());

		if (m_inv.is_active())
		{
			m_nb_slots = m_inv.get_nb_slots();
			m_board.get_hitbox().set_extent(m_board.get_texture()->get_size());
			m_board.set_pos(v2f32{ Window::g_gui_view_size.x / 2, Window::g_gui_view_size.y / 2 - m_hotbar.get_size().y });

			const auto target_size_inv = m_nb_slots.x * m_nb_slots.y;

			types::pos2d start_pos{ m_board.get_pos() - m_board.get_size() + g_outline };
			types::pos2d slot_pos{ start_pos };
			for (i32 y{}; y < m_nb_slots.y; y++)
			{
				slot_pos.x = start_pos.x;
				for (i32 x{}; x < m_nb_slots.x; x++)
				{
					const auto current_index = x + y * m_nb_slots.x;
					update_item_stack(m_item_stacks, target_size_inv, current_index, m_inv.get_item_stack(current_index), slot_pos);

					slot_pos.x += g_slot_size;
				}

				slot_pos.y += g_slot_size;
			}
		}

		m_hotbar.get_hitbox().set_extent(m_hotbar.get_texture()->get_size());
		m_hotbar.set_pos(v2f32{ Window::g_gui_view_size.x / 2, Window::g_gui_view_size.y - m_hotbar.get_size().y });

		types::pos2d slot_pos{ m_hotbar.get_pos() - m_hotbar.get_size() + g_outline_hb };
		for (i32 x{}; x < m_inv.get_nb_slots_hb(); x++)
		{
			update_item_stack(m_item_stacks_hb, m_inv.get_nb_slots_hb(), x, m_inv.get_item_stack_hb(x), slot_pos);
			slot_pos.x += g_slot_size;
		}

	}

	std::optional<size_t> InventoryGUI::compute_index(types::pos2d gui_mouse_pos) noexcept
	{
		phy::HitboxAABB2D board{ m_board.get_pos(), m_board.get_size() - g_outline };

		if (phy::intersects(board, gui_mouse_pos))
		{
			v2f32 converted_mp{ gui_mouse_pos - board.get_min() };
			v2u64 index2d{ static_cast<size_t>(converted_mp.x / (g_slot_size + 1)), static_cast<size_t>(converted_mp.y / (g_slot_size + 1)) };

			return std::make_optional(index2d.x + index2d.y * m_nb_slots.x);
		}
		else
			return std::nullopt;
	}

	std::optional<size_t> InventoryGUI::compute_index_hb(types::pos2d gui_mouse_pos) noexcept
	{
		phy::HitboxAABB2D hotbar{ m_hotbar.get_pos(), m_hotbar.get_size() - g_outline_hb };

		if (phy::intersects(hotbar, gui_mouse_pos))
		{
			v2f32 converted_mp{ gui_mouse_pos - hotbar.get_min() };
			v2u64 index2d{ static_cast<size_t>(converted_mp.x / (g_slot_size + 1)), {} };

			return std::make_optional(index2d.x);
		}
		else
			return std::nullopt;
	}



}