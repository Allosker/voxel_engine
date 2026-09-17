#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Define a simple 2D rectangle that has a mesh, transform, texture, and hitbox
* ==============================================-
*/

#include "gfx/mesh.hpp"
#include "gfx/texture.hpp"
#include "gfx/transformable2D.hpp"
#include "gfx/vertices.hpp"
#include "gfx/drawable.hpp"
#include "gfx/material.hpp"

#include "sys/types.hpp"

#include "sys/assetsManager.hpp"

#include "phy/hitboxAABB_2D.hpp"


namespace gfx
{
	struct RenderContext;

	class Rectangle
		: public Transformable2D, public gfx::Drawable
	{
	public:

		Rectangle(v2f32 extent)
			: m_hitbox{ types::pos2d{}, extent },
			m_material{&AssetsManager::get().shaders.at("shaders/twoD")}
		{
			m_mesh.create_buffer<Vertex2D>();
		}

		/// <summary>
		/// Update the GPU buffer based on the size of the texture, so it is default-sized
		/// </summary>
		/// <param name="tex"></param>
		void update_sprite(const Texture* tex) noexcept;

		void set_size(v2f32 size) noexcept { m_size = size; }


		const phy::HitboxAABB2D& get_hitbox() const noexcept { return m_hitbox; }
		phy::HitboxAABB2D& get_hitbox() noexcept { return m_hitbox; }

		v2f32 get_size() const noexcept { return m_size * get_scale(); }


		/// <summary>
		/// If there is no attached texture, we do not draw the object, since the object is defined by its texture
		/// </summary>
		void draw(Renderer& renderer) noexcept;


	private:

		Mesh m_mesh;
		Material m_material;

		phy::HitboxAABB2D m_hitbox;

		v2f32 m_size{};
	};


}