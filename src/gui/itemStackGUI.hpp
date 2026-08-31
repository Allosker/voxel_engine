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

#include "gfx/voxel.hpp"


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
	{
	public:

		ItemStackGUI() noexcept
		{
			std::vector<gfx::Vertex> mesh{};

			for (const auto& i : gfx::Voxel::model)
				assemble_pos_uvs(
					mesh,
					i,
					gfx::Voxel::face_uvs
				);

			m_mesh.create_buffer<gfx::Vertex>(false);
			m_mesh.update_buffer(mesh, GL_STREAM_DRAW);

			m_trans.set_scale(1.f);
			m_trans.move(v3f64{ 0, -1, 0 });
			
		}


		void draw(const gfx::RenderContext& rc) noexcept
		{
			//m_trans.set_rotation(qf64::fromAxis({ 0, 1, 0 }, angle32::from_degrees(120.0f)));

			rc.sha->set_value("model", m_trans.get_transform());

			rc.tex->bind();

			m_mesh.draw();

			rc.tex->unbind();
		}


	private:

		gfx::Mesh m_mesh;

		gfx::Transformable3D m_trans{};


	};

}