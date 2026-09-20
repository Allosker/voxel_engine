#pragma once 
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Simple definition of a world entity, a world entity possesses a mesh, an AABB hitbox, and a transform
* It exposes a common interface (it is meant to be derived from), so that all entities can be updated the same way
* ==============================================-
*/

#include "gfx/drawable.hpp"
#include "gfx/mesh.hpp"
#include "gfx/renderer.hpp"
#include "gfx/transformable3D.hpp"
#include "gfx/voxel.hpp"
#include "material.hpp"
#include "phy/hitboxAABB.hpp"
#include "sys/assetsManager.hpp"
#include "sys/hash.hpp"
#include "sys/types.hpp"
#include <cassert>
#include <vector>


namespace gfx
{

	class WorldItem
		: public Transformable3D
	{
	public:

		WorldItem(types::type_id id, const types::pos& pos)
			: Transformable3D{ pos }, m_hitbox{ pos, v3f64{ 0.2 } }
		{
			set_scale(0.2);
			set_pos(pos + 0.5 - get_scale() / 2.);
		}

		DEFAULT_COPY_INIT(WorldItem);
		DEFAULT_MOVE_INIT(WorldItem);

		types::type_id get_id() const noexcept { return m_id; }


	private:

		phy::HitboxAABB m_hitbox;
		types::type_id m_id;


	};


	class WorldItemMesh
		: public Drawable
	{
	public:

		WorldItemMesh(const WorldItem& wi)
			: m_wi{ wi }, m_material{ &AssetsManager::get().shaders.at("shaders/world_entities"_id) }
		{
			assert(wi && "ERROR::WorldItemMesh::World Items Mesh requires a non-null world item");

			m_mesh.create_buffer<Vertex>(false);

			std::vector<Vertex> mesh{};

			const std::vector<v2f32>& uvs = calculate_uvs(m_wi.get_id());
			for (const auto& i : Voxel::g_model)
				assemble_pos_uvs<Vertex>(
					mesh,
					i,
					uvs
				);

			m_mesh.update_buffer(mesh, GL_STATIC_DRAW);

			m_material.set("tex", &AssetsManager::get().textures.at("textures/voxels/atlas"_id));
		}

		DELETE_COPY_INIT(WorldItemMesh);
		DEFAULT_MOVE_INIT(WorldItemMesh);


		void draw(Renderer& renderer) noexcept override
		{
			renderer.push_command(&m_mesh, static_cast<m4f32>(m_wi.get_transform()), &m_material, RenderLayer::Opaque);
		}


	private:

		Material m_material;
		Mesh m_mesh{};

		const WorldItem& m_wi;


	};


}