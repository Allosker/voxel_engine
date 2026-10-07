#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Based on the PlayerInventory class, this small class is used to display the little menu icons and tell whether one has been pressed
* ==============================================-
*/

#include "gfx/drawable.hpp"
#include "gfx/playerInventory.hpp"
#include "gfx/rectangle.hpp"
#include "sys/assetsManager.hpp"
#include "sys/inputManager.hpp"
#include "sys/window.hpp"
#include <gfx/renderer.hpp>
#include <print>
#include <sys/event.hpp>

#include "gfx/debugRenderer.hpp"

namespace gui
{

	class PI_Selection
		: public gfx::Drawable
	{

		struct SelectRect
			: public gfx::Rectangle
		{
			SelectRect(const gfx::Texture* tex)
				: Rectangle(tex)
			{ }

			bool on_pointer{};

			void check_for_intersection(types::pos2d pos)
			{
				auto hitbox = get_hitbox();
				hitbox.set_pos(get_pos());
				on_pointer = phy::intersects(hitbox, pos);
			}
		};

	public:

		PI_Selection(gfx::PlayerInventory& pi)
			: m_inv{ &AssetsManager::get().textures.at("textures/gui/inventory_select"_id) },
			m_craft{ &AssetsManager::get().textures.at("textures/gui/crafting_select"_id) },
			m_armor{ &AssetsManager::get().textures.at("textures/gui/armor_select"_id) }, m_pi{ pi }
		{
			m_dh = sys::InputManager::get().subscribe(&PI_Selection::on_click, *this, Event::MouseButtonEvent{});

			m_armor.set_pos(g_p_left);
			m_inv.set_pos(g_p_mid);
			m_craft.set_pos(g_p_right);
		}

		~PI_Selection()
		{
			sys::InputManager::get().unsubscribe(m_dh);
		}  


		void update(types::pos2d mouse_gui) noexcept
		{
			m_inv.check_for_intersection(mouse_gui);
			m_craft.check_for_intersection(mouse_gui);
			m_armor.check_for_intersection(mouse_gui);
		}

		void on_click(Event::MouseButtonEvent event) noexcept
		{
			if (!m_pi.is_active()) return;

			if (event.scancode == MouseButtons::Left)
			{
				if (event.state == Event::ButtonState::Press)
				{
					if (m_inv.on_pointer)
					{
						m_pi.set_state(gfx::PlayerInventory::Inventory);

						m_armor.set_pos(g_p_left);
						m_inv.set_pos(g_p_mid);
						m_craft.set_pos(g_p_right);

						m_inv.set_scale(1.f);
						m_craft.set_scale(0.8f);
						m_armor.set_scale(0.8f);
					}

					if (m_craft.on_pointer)
					{
						m_pi.set_state(gfx::PlayerInventory::Crafting);

						m_inv.set_pos(g_p_left);
						m_craft.set_pos(g_p_mid);
						m_armor.set_pos(g_p_right);

						m_craft.set_scale(1.f);
						m_inv.set_scale(0.8f);
						m_armor.set_scale(0.8f);
					}

					if (m_armor.on_pointer)
					{
						m_pi.set_state(gfx::PlayerInventory::Armor);

						m_craft.set_pos(g_p_left);
						m_armor.set_pos(g_p_mid);
						m_inv.set_pos(g_p_right);

						m_armor.set_scale(1.f);
						m_inv.set_scale(0.8f);
						m_craft.set_scale(0.8f);
					}
				}
			}
		}


		void draw(gfx::Renderer& renderer) noexcept
		{
			if (!m_pi.is_active()) return;

			m_inv.draw(renderer);
			m_craft.draw(renderer);
			m_armor.draw(renderer);

		}


	private:

		static constexpr f32 g_size_select{ 29.f }; // in pixels
		static constexpr types::pos2d g_p_right{ Window::g_gui_view_size.x * 0.5f + g_size_select * 2.5f, g_size_select * 1.2f};
		static constexpr types::pos2d g_p_left{ Window::g_gui_view_size.x * 0.5f - g_size_select * 2.5f, g_size_select * 1.2f };
		static constexpr types::pos2d g_p_mid{ Window::g_gui_view_size.x * 0.5f, g_size_select * 1.2f };

		SelectRect m_inv;
		SelectRect m_craft;
		SelectRect m_armor;

		sys::InputManager::DelegateHandle m_dh{};

		gfx::PlayerInventory& m_pi;


	};

}