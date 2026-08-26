#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Graphical representation of the Inventory of the player.
* ==============================================-
*/

#include "gfx/inventory.hpp"
#include "gfx/shader.hpp"
#include "sys/assetsManager.hpp"
#include "gfx/rectangle.hpp"


namespace gui
{

	class InventoryGUI
	{
	public:

		InventoryGUI()
			: m_size{ gfx::Inventory::Size::Small }, m_rec{ {} }
		{  
			set_texture(m_size);

			m_rec.get_hitbox()
			
		}

		void update(const gfx::Inventory& inv) noexcept
		{
			if (inv.get_size() != m_size)
			{
				m_size = inv.get_size();
				set_texture(m_size);
			}


		}



		void draw(const gfx::Shader& shader) noexcept
		{
			shader.set_value("model", m_trans.get_transform());


			m_tex->bind();

			m_mesh.draw();

			m_tex->unbind();
		}



	private:

		void set_texture(gfx::Inventory::Size size) noexcept
		{
			switch (size)
			{
				case gfx::Inventory::Size::Small:
					m_tex = &AssetsManager::get().textures.at("textures/gui/inventory/small");
					break;

				case gfx::Inventory::Size::Medium:
					m_tex = &AssetsManager::get().textures.at("textures/gui/inventory/medium");
					break;

				case gfx::Inventory::Size::Big:
					m_tex = &AssetsManager::get().textures.at("textures/gui/inventory/big");
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