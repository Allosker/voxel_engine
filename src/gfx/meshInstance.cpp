#include "MeshInstance.hpp"

#include "renderer.hpp"

namespace gfx
{

	MeshInstance::MeshInstance(Mesh* mesh, Shader* shader) : m_mesh{ mesh }, m_material{ shader }
	{

	}

	void MeshInstance::draw(Renderer& renderer)
	{
		renderer.push_command(m_mesh, static_cast<m4f32>(get_transform()), &m_material, RenderLayer::Opaque);
	}

}
