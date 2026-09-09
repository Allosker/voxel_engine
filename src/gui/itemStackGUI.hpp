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
#include "sys/assetsManager.hpp"


namespace gui
{

	static void assemble_pos_uvs(auto& out, const auto& pos, const auto& uvs) noexcept
	{
		out.emplace_back(gfx::Vertex{ pos[0], uvs[0] });
		out.emplace_back(gfx::Vertex{ pos[1], uvs[1] });
		out.emplace_back(gfx::Vertex{ pos[2], uvs[2] });
		out.emplace_back(gfx::Vertex{ pos[3], uvs[3] });
		out.emplace_back(gfx::Vertex{ pos[4], uvs[4] });
		out.emplace_back(gfx::Vertex{ pos[5], uvs[5] });
	}

	/// <summary>
	/// 
	/// </summary>
	class ItemStackGUI
		: public gfx::Transformable3D
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
			: m_text{ &AssetsManager::get().fonts.at("fonts/november") }
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


		void draw_model(const gfx::RenderContext& rc) noexcept
		{
			if (!m_should_be_drawn) return;

			rc.sha->set_value("model", get_transform());

			rc.tex->bind();

			m_mesh.draw();

			rc.tex->unbind();
		}

		void draw_text(const gfx::Shader& text_sha) noexcept
		{
			if (!m_should_be_drawn) return;

			m_text.draw(text_sha);
		}


	private:
		
		static constexpr f32 g_x_size_three_digits_number_px{ 36.f }; 


		void update_model(types::type_id id)
		{
			std::vector<gfx::Vertex> mesh{};

			for (const auto& i : g_model)
				assemble_pos_uvs(
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

		gfx::Text m_text;
		
		types::type_id m_id{};

		bool m_should_be_drawn{};


	};

}