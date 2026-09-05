#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Define an instance of a MeshInstance, that can be renderer

* ==============================================-
*/

#include "transformable3D.hpp"
#include "material.hpp"


namespace gfx
{
	class Mesh;
	class Shader;
	class Texture;

	class MeshInstance : public Transformable3D
	{
	public:
		MeshInstance(Mesh* mesh, Shader* shader);

		MeshInstance(const MeshInstance&) = default;
		MeshInstance(MeshInstance&& other) = default;

		MeshInstance& operator=(const MeshInstance&) = default;
		MeshInstance& operator=(MeshInstance&& other) = default;

		Mesh* m_mesh{};
		Material m_material;
	};

} // gfx