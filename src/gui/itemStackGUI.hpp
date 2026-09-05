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
		{
			m_mesh.create_buffer<gfx::Vertex>(false);
		}

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

		void set_should_be_drawn(bool b) noexcept { m_should_be_drawn = b; }


		void draw(const gfx::RenderContext& rc) noexcept
		{
			if (!m_should_be_drawn) return;

			rc.sha->set_value("model", get_transform());

			rc.tex->bind();

			m_mesh.draw();

			rc.tex->unbind();
		}


	private:

		gfx::Mesh m_mesh;

		bool m_should_be_drawn{};


	};

}