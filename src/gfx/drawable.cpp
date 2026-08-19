#include "drawable.hpp"

namespace gfx
{

	// =====================
	// Actors
	// =====================

	bool Drawable::draw() noexcept
	{
		if (m_contexts.empty() || !m_camera) return false;


		m_shader.bind();
		m_shader.setValue("vp", m_camera->get_VP());

		for (const auto& i : m_contexts)
		{
			if (!i.mesh) continue;

			m_shader.setValue("model", i.model);

			if (i.texture)
				i.texture->bind();

			i.mesh->bind();
			glDrawElements(i.draw_mode, i.mesh->getNumberIndices(), GL_UNSIGNED_INT, 0);
			i.mesh->unbind();

			if (i.texture)
				i.texture->unbind();

		}

		m_shader.unbind();


		return true;
	}

}