#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Define several vertex types to provide a Mesh with
* ==============================================-
*/

#include "sys/types.hpp"

#define DEFINE_VERTEX_VAR(index, Class, var) \
glVertexAttribPointer(index, VertexTrait<decltype(Class :: var)>::length, VertexTrait<decltype(Class :: var)>::gl_type, false, sizeof(Class), std::bit_cast<void*>(offsetof(Class, var)));  \
glEnableVertexAttribArray(index);

namespace gfx
{
	template <typename T>
	struct VertexTrait
	{
		static_assert(sizeof(T) == 0, "Unsupported type for VertexTrait.");
	};

	template <>
	struct VertexTrait<float>
	{
		static constexpr GLenum gl_type = GL_FLOAT;
		static constexpr GLint length = 1;
	};

	template <>
	struct VertexTrait<v2f32>
	{
		static constexpr GLenum gl_type = GL_FLOAT;
		static constexpr GLint length = 2;
	};

	template <>
	struct VertexTrait<v3f32>
	{
		static constexpr GLenum gl_type = GL_FLOAT;
		static constexpr GLint length = 3;
	};

	template <>
	struct VertexTrait<v4f32>
	{
		static constexpr GLenum gl_type = GL_FLOAT;
		static constexpr GLint length = 4;
	};

	struct Vertex
	{
		v3f32 pos;
		v2f32 uvs;

		static void setupAttributes()
		{
			DEFINE_VERTEX_VAR(0, Vertex, pos);
			DEFINE_VERTEX_VAR(1, Vertex, uvs);
		}
	};

	struct VertexRGBA
	{
		v3f32 pos;
		v4f32 color;

		static void setupAttributes()
		{
			DEFINE_VERTEX_VAR(0, VertexRGBA, pos);
			DEFINE_VERTEX_VAR(1, VertexRGBA, color);
		}
	};

	struct Vertex2D
	{
		v2f32 pos;
		v2f32 uvs;

		static void setupAttributes()
		{
			DEFINE_VERTEX_VAR(0, Vertex2D, pos);
			DEFINE_VERTEX_VAR(1, Vertex2D, uvs);
		}
	};

	struct Vertex2DRGBA
	{
		v2f32 pos;
		v4f32 color;

		static void setupAttributes()
		{
			DEFINE_VERTEX_VAR(0, Vertex2DRGBA, pos);
			DEFINE_VERTEX_VAR(1, Vertex2DRGBA, color);
		}
	};

}