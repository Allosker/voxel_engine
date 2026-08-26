#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Graphical representation of the Inventory of the player.
* ==============================================-
*/

#include "gfx/inventory.hpp"
#include "sys/assetsManager.hpp"


namespace gui
{

	class InventoryGUI
	{
	public:

		InventoryGUI()
			: m_size{ gfx::Inventory::Size::Small }
		{
			set_texture(m_size);

			m_trans.set_scale(1.5f);
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

		gfx::Transformable2D m_trans;
		const gfx::Texture* m_tex;
		gfx::Mesh m_mesh{};

		gfx::Inventory::Size m_size{};


	};

}