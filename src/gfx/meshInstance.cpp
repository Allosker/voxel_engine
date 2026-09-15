#include "MeshInstance.hpp"

#include "renderer.hpp"

namespace gfx
{

	MeshInstance::MeshInstance(Mesh* mesh, Shader* shader) : m_mesh{ mesh }, m_material{ shader }
	{

	}

	void MeshInstance::draw(Renderer& renderer)
	{
		renderer.push_command({.mesh = m_mesh, .transform = get_transform(), .material = &m_material});
	}
}
