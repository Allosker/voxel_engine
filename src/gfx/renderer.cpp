#include "renderer.hpp"

#include "mesh.hpp"
#include "camera.hpp"
#include "material.hpp"

namespace gfx
{

	void Renderer::start(const Camera& camera)
	{
		m_camera = camera;
		m_viewMatrix = m_camera.get_VP();
		m_viewZRow = { m_viewMatrix[0][2], m_viewMatrix[1][2], m_viewMatrix[2][2], m_viewMatrix[3][2] };

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::draw()
	{
		glEnable(GL_DEPTH_TEST);

		GlobalUniformBlockInstance viewUBI{ *Shader::FindGlobalUniformBlockDefinition("ViewData") };
		viewUBI.set("vp", m_viewMatrix);

		viewUBI.update();
		viewUBI.bind();

		GlobalUniformBlockInstance instanceUBI{ *Shader::FindGlobalUniformBlockDefinition("InstanceData") };
		instanceUBI.bind();

		std::sort(m_commands.begin(), m_commands.end());

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

		m_commands.clear();
	}

}