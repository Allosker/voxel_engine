#pragma once 
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Simple definition of a world entity, a world entity possesses a mesh, an AABB hitbox, and a transform
* It exposes a common interface (it is meant to be derived from), so that all entities can be updated the same way
* ==============================================-
*/

#include "gfx/transformable3D.hpp"
#include "gfx/mesh.hpp"
#include "gfx/voxel.hpp"
#include "gfx/renderContext.hpp"


namespace gfx
{


	class WorldItem
		: public Transformable3D
	{
	public: 

		WorldItem(types::type_id id, const types::pos& pos)
			: Transformable3D{ pos }
		{
			m_mesh.create_buffer<Vertex>(false);

			std::vector<gfx::Vertex> mesh{};

			for (const auto& i : Voxel::g_model)
				assemble_pos_uvs<gfx::Vertex>(
					mesh,
					i,
					gfx::calculate_uvs(id)
				);
			m_mesh.update_buffer(mesh, GL_STATIC_DRAW);
		}


		void draw(const gfx::RenderContext& rc) noexcept
		{
			rc.sha->set_value("model", get_transform());

			m_mesh.draw();
		}


	private:

		Mesh m_mesh{};

	};


}