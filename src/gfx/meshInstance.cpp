#include "MeshInstance.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace gfx
{

	MeshInstance::MeshInstance(Mesh* mesh, Shader* shader, Texture* texture) : m_mesh{ mesh }, m_shader{ shader }, m_texture{ texture }
	{

	}

}
