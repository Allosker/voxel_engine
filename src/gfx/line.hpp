#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Implement basic 3D lines (for now)
* ==============================================-
*/

#include "drawable.hpp"
#include "gfx/material.hpp"
#include "gfx/mesh.hpp"
#include "gfx/transformable3D.hpp"
#include "renderer.hpp"
#include "sys/assetsManager.hpp"
#include "sys/hash.hpp"
#include "vertices.hpp"


namespace gfx
{

	class Line
		: public Transformable3D, public Drawable
	{
	public:

		struct LineVertex
		{
			v3f32 pos_a;
			v3f32 pos_b;
			v4f32 rgba;

			static void setupAttributes()
			{
				DEFINE_VERTEX_VAR(0, LineVertex, pos_a);
				DEFINE_VERTEX_VAR(1, LineVertex, pos_b);
				DEFINE_VERTEX_VAR(2, LineVertex, rgba);
			}
		};

		Line() noexcept
			: m_material{ &AssetsManager::get().shaders.at("shaders/line"_id) }
		{
			m_mesh.create_buffer<LineVertex>(false);

			m_mesh.update_buffer<LineVertex>({ { { 0., 0., 0. }, { 1., 30., 0. }, { 1., 1., 1., 1.} } });
		}


		void draw(Renderer& renderer) noexcept
		{
			renderer.push_command(&m_mesh, static_cast<m4f32>(get_transform()), &m_material, RenderLayer::Transparent);
		}


	private:

		Material m_material;

		Mesh m_mesh;


	};


}