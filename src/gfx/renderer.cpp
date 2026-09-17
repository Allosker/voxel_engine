#include "renderer.hpp"

#include "mesh.hpp"
#include "camera.hpp"
#include "material.hpp"

namespace gfx
{

	void Renderer::start(const Camera& camera, m4f32 uiViewMatrix)
	{
		m_camera = camera;
		m_viewMatrix = m_camera.get_VP();
		m_viewZRow = { m_viewMatrix[0][2], m_viewMatrix[1][2], m_viewMatrix[2][2], m_viewMatrix[3][2] };

		m_uiViewMatrix = uiViewMatrix;

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::draw()
	{
		glEnable(GL_DEPTH_TEST);

		viewUBI.bind();
		instanceUBI.bind();

		std::sort(m_commands.begin(), m_commands.end());

		Shader* currentShader{};
		RenderLayer currentLayer = RenderLayer::COUNT;

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

			const auto newLayer = command.key.getRenderLayer();
			if (newLayer != currentLayer)
			{
				currentLayer = newLayer;

				if (currentLayer == RenderLayer::Opaque)
				{
					glEnable(GL_DEPTH_TEST);
					glDepthMask(GL_TRUE);
					glDisable(GL_BLEND);
					glEnable(GL_CULL_FACE);

					viewUBI.set("vp", m_viewMatrix);
					viewUBI.update();
				}
				else if (currentLayer == RenderLayer::Transparent)
				{
					glEnable(GL_DEPTH_TEST);
					glDepthMask(GL_FALSE);
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					glDisable(GL_CULL_FACE);

					viewUBI.set("vp", m_viewMatrix);
					viewUBI.update();
				}
				else if (currentLayer == RenderLayer::UI)
				{
					glDisable(GL_DEPTH_TEST);
					glDepthMask(GL_FALSE);
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					glDisable(GL_CULL_FACE);

					viewUBI.set("vp", m_uiViewMatrix);
					viewUBI.update();
				}
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