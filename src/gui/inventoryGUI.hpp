#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Graphical representation of the Inventory of the player.
* ==============================================-
*/

#include "gfx/inventory.hpp"
#include "gfx/rectangle.hpp"
#include "gfx/shader.hpp"
#include "sys/assetsManager.hpp"

#include "gfx/debugRenderer.hpp"


namespace gui
{

	class InventoryGUI
	{
	public:

		InventoryGUI()
			: m_size{ gfx::Inventory::Size::Small }, m_rec{ {} }
		{  
			set_texture(m_size);

			m_rec.get_hitbox().set_extent(m_rec.get_texture()->get_size());
		}

		void update(const gfx::Inventory& inv) noexcept
		{
			if (inv.get_size() != m_size)
			{
				m_size = inv.get_size();
				set_texture(m_size);
			}

			gfx::aabb_min_max(v3f32{ m_rec.get_hitbox().get_min(), 0.f }, v3f32{ m_rec.get_hitbox().get_max(), 0.f }, { 1., 1., 1.}, 0.f, true);

		}



		void draw(const gfx::Shader& shader) noexcept
		{
			m_rec.draw(shader);
		}



	private:

		void set_texture(gfx::Inventory::Size size) noexcept
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


	private:

		gfx::Rectangle m_rec;

		gfx::Inventory::Size m_size{};


	};

}