#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Graphical representation of the Inventory of the player.
* ==============================================-
*/

#include <optional>

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
			: m_size{ gfx::Inventory::Size::None }, m_rec{ {} }
		{  
			m_rec.transform().set_scale(g_scale);
		}


		void update(const gfx::Inventory& inv, types::pos2d gui_mouse_pos) noexcept;

		std::optional<size_t> get_slot_index() const noexcept { return m_index; }


		void draw(const gfx::Shader& shader) noexcept
		{
			m_rec.draw(shader);
		}



	private:

		void change_inventory(gfx::Inventory::Size size) noexcept;

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

		/// <summary>
		/// Changes according to the inventory's stage
		/// </summary>
		v2f32 m_slot_size{};


	private:

		gfx::Rectangle m_rec;

		gfx::Inventory::Size m_size{};
		v2f32 m_nb_slots{};

		std::optional<size_t> m_index{};


	};

}