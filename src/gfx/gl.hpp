#pragma once // texture.hpp
// MIT
// Allosker ------------------------------
// =========+
// Implement a small class that allocates a texture on the GPU (be it an atlas or anything). Based on a dynamic array of image data.
// ---------------------------------------

#include "sys/graphics.hpp"
#include "sys/types.hpp"

#include "image.hpp"


namespace gfx
{
	template<auto creater, auto deleter>
	struct GlId
	{
		GlId() : m_id{creater()}
		{

		}

		GlId(GlId&& other)
		{
			m_id = other.m_id;
			other.m_id = -1;
		}

		GlId& operator=(GlId&& other)
		{
			m_id = other.m_id;
			other.m_id = -1;
		}

		~GlId()
		{
			deleter(m_id);
		}

		GLuint m_id = -1;

		operator GLuint() const
		{
			return m_id;
		}
	};

	using GlId_Shader = GlId<[](){ return glCreateProgram();}, [](auto id) {glDeleteProgram(id);}>;

} // render