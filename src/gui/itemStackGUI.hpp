#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Define any in-game object and make it a 2D image that can be displayed to represent an Item.
* Can do that from a 3D model or a 
* ==============================================-
*/  

#include "gfx/mesh.hpp"
#include "gfx/renderContext.hpp"
#include "gfx/transformable3D.hpp"
#include "gfx/voxel.hpp"
#include "gfx/text.hpp"
#include "gfx/drawable.hpp"

#include "sys/assetsManager.hpp"


namespace gui
{

	/// <summary>
	/// 
	/// </summary>
	class ItemStackGUI
		: public gfx::Transformable3D, public gfx::Drawable
	{
		static constexpr std::array<std::array<v3f32, 6>, 6> g_model
		{
			/*Left*/
			std::array<v3f32, 6>
			{
				v3f32
				{ 0.5, -0.5, 0.5 },  /*Left-Down*/
				{ 0.5, -0.5, -0.5 },   /*Right-Down*/
				{ 0.5, 0.5, 0.5 },  /*Left-Up*/

				{ 0.5, -0.5, -0.5 },   /*Right-Down*/
				{ 0.5, 0.5, -0.5 },   /*Right-Up*/
				{ 0.5, 0.5, 0.5 },  /*Left-Up*/
			},

			// /*Right*/ 
			std::array<v3f32, 6>
			{
				v3f32
				{ -0.5, -0.5, -0.5 },  /*Right-Down*/
				{ -0.5, -0.5, 0.5 },   /*Left-Down*/
				{ -0.5, 0.5, -0.5 },  /*Right-Up*/

				{ -0.5, -0.5, 0.5 },   /*Left-Up*/
				{ -0.5, 0.5, 0.5 },  /*Right-Up*/
				{ -0.5, 0.5, -0.5 },   /*Left-Down*/
			},


			/*Up*/
			std::array<v3f32, 6>
			{
				v3f32
				{ 0.5, 0.5, 0.5 },   /*Left-Down*/
				{ 0.5, 0.5, -0.5 },  /*Right-Down*/
				{ -0.5, 0.5, 0.5 },   /*Left-Up*/

				{ 0.5, 0.5, -0.5 },  /*Right-Down*/
				{ -0.5, 0.5, -0.5 },  /*Right-Up*/
				{ -0.5, 0.5, 0.5 },   /*Left-Up*/
			},

			/*Down*/
			std::array<v3f32, 6>
			{
				v3f32
				{ 0.5, -0.5, -0.5 },  /*Left-Down*/
				{ 0.5, -0.5, 0.5 },  /*Right-Down*/
				{ -0.5, -0.5, -0.5 },   /*Left-Up*/

				{ 0.5, -0.5, 0.5 },  /*Right-Down*/
				{ -0.5, -0.5, 0.5 },   /*Right-Up*/
				{ -0.5, -0.5, -0.5 },   /*Left-Up*/
			},


			/*Front*/
			std::array<v3f32, 6>
			{
				v3f32
				{ -0.5, -0.5, 0.5},  /*Left-Down*/
				{ 0.5, -0.5, 0.5 },  /*Right-Down*/
				{ -0.5, 0.5, 0.5 },  /*Left-Up*/

				{ 0.5, -0.5, 0.5 },  /*Right-Down*/
				{ 0.5, 0.5, 0.5 },  /*Right-Up*/
				{ -0.5, 0.5, 0.5 },  /*Left-Up*/
			},

			/*Back*/
			std::array<v3f32, 6>
			{
				v3f32
				{ 0.5, -0.5, -0.5 },  /*Left-Down*/
				{ -0.5, -0.5, -0.5 },  /*Right-Down*/
				{ 0.5, 0.5, -0.5 },  /*Left-Up*/

				{ -0.5, -0.5, -0.5 },  /*Right-Down*/
				{ -0.5, 0.5, -0.5 },  /*Right-Up*/
				{ 0.5, 0.5, -0.5 },  /*Left-Up*/
			},


		};


	public:

		ItemStackGUI() noexcept
			: m_text{ &AssetsManager::get().fonts.at("fonts/november") },
			m_material{&AssetsManager::get().shaders.at("shaders/text")}
		{
			m_mesh.create_buffer<gfx::Vertex>(false);
			m_text.set_scale(0.5);
		}

		/// <summary>
		/// Make it so that it accounts for the origin of the id
		/// </summary>
		void update(gfx::ItemStack::Type type, const std::string& str) noexcept
		{
			if (type.id != m_id)
			{
				m_id = type.id;
				update_model(type.id);
			}

			if (str != m_text.get_str())
				set_text(str);
		}

		void set_should_be_drawn(bool b) noexcept { m_should_be_drawn = b; }

		void set_pos(const types::pos& pos) noexcept override
		{
			Transformable3D::set_pos(pos);
			set_text_pos();
		}

		void set_scale(f64 scale) noexcept override
		{
			Transformable3D::set_scale(scale);
			set_text_pos();
		}

		void set_scale_text(f64 scale) noexcept
		{
			m_text.set_scale(scale);
		}

		void set_text_pos() noexcept
		{
			m_text.set_pos({ get_pos().x + g_x_size_three_digits_number_px - m_text.get_size().x, get_pos().y + get_scale().y * 0.3, 0. });
		}


		bool should_be_drawn() const noexcept { return m_should_be_drawn; }


		void draw(gfx::Renderer& renderer) noexcept
		{
			if (!m_should_be_drawn) return;

			renderer.push_command(&m_mesh, m3f32{}, &m_material);
			m_text.draw(renderer);
		}

	private:
		
		static constexpr f32 g_x_size_three_digits_number_px{ 36.f }; 


		void update_model(types::type_id id)
		{
			std::vector<gfx::Vertex> mesh{};

			for (const auto& i : g_model)
				gfx::assemble_pos_uvs<gfx::Vertex>(
					mesh,
					i,
					gfx::calculate_uvs(id)
				);

			m_mesh.update_buffer(mesh, GL_STREAM_DRAW);
		}

		void set_text(const std::string& str) noexcept
		{
			m_text.set_str(str);
		}


	private:

		gfx::Mesh m_mesh;
		gfx::Material m_material;
		gfx::Text m_text;
		
		types::type_id m_id{};

		bool m_should_be_drawn{};


	};

}