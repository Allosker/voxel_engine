#include "renderer.hpp"

#include "mesh.hpp"
#include "camera.hpp"
#include "material.hpp"

namespace gfx
{
	void Renderer::draw(const Camera& camera)
	{
		glEnable(GL_DEPTH_TEST);

		GlobalUniformBlockInstance viewUBI{ *Shader::FindGlobalUniformBlockDefinition("ViewData") };
		viewUBI.set("vp", (m4f32)camera.get_VP());

		viewUBI.update();
		viewUBI.bind();

		GlobalUniformBlockInstance instanceUBI{ *Shader::FindGlobalUniformBlockDefinition("InstanceData") };
		instanceUBI.bind();

		Shader* currentShader{};

		for (const auto& command : m_commands)
		{
			if (!command.mesh || !command.material)
			{
				continue;
			}

			auto* newShader = &command.material->get_shader();

			if (newShader != currentShader)
			{
				if (currentShader)
				{
					currentShader->unbind();
				}

				currentShader = newShader;
				currentShader->bind();
			}

			instanceUBI.set("model", command.transform);
			instanceUBI.update();

			command.material->updateBlocks();

			command.material->bindBlocks();
			command.material->bindTextures();

			command.mesh->draw();
		}

		m_commands.resize(0);
	}

}