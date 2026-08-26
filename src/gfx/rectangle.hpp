#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Define a simple 2D rectangle that has a mesh, transform, texture, and hitbox
* ==============================================-
*/

#include "gfx/transformable2D.hpp"
#include "gfx/mesh.hpp"
#include "gfx/shader.hpp"
#include "gfx/texture.hpp"
#include "gfx/vertices.hpp"

#include "phy/hitboxAABB_2D.hpp"


namespace gfx
{
	
	class Rectangle
	{	
	public:

		Rectangle(v2f32 extent)
			: m_hitbox{ types::pos2d{}, extent }
		{
			m_mesh.create_buffer<Vertex2D>({}, {}, GL_STATIC_DRAW);
		}
		
		/// <summary>
		/// Update the GPU buffer based on the size of the texture, so it is default-sized
		/// </summary>
		/// <param name="tex"></param>
		void update_sprite(const Texture* tex) noexcept;

		/// <summary>
		/// Get access to the transform to move/scale/rotate the object
		/// </summary>
		/// <returns></returns>
		Transformable2D& transform() noexcept { return m_trans; }

		const phy::HitboxAABB2D& get_hitbox() const noexcept { return m_hitbox; }


		/// <summary>
		/// If there is no attached texture, we do not draw the object, since the object is defined by its texture
		/// </summary>
		void draw(const Shader& shader) noexcept;


	private:

		Transformable2D m_trans{};

		Mesh m_mesh;

		phy::HitboxAABB2D m_hitbox;

		const Texture* m_tex{ nullptr };


	};


}