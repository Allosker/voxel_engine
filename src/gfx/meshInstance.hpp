#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Define an instance of a MeshInstance, that can be renderer

* ==============================================-
*/

#include "transformable3D.hpp"


namespace gfx
{
	class Mesh;
	class Shader;
	class Texture;

	class MeshInstance : public Transformable3D
	{
	public:
		MeshInstance(Mesh* mesh = nullptr, Shader* shader = nullptr, Texture* texture = nullptr);

		MeshInstance(const MeshInstance&) = default;
		MeshInstance(MeshInstance&& other) = default;

		MeshInstance& operator=(const MeshInstance&) = default;
		MeshInstance& operator=(MeshInstance&& other) = default;

		Mesh* m_mesh{};
		Shader* m_shader{};
		Texture* m_texture{};		
	};

} // gfx