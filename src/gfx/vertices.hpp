#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Define several vertex types to provide a Mesh with
* ==============================================-
*/

#include "sys/types.hpp"


namespace gfx
{

	struct Vertex
	{
		v3f32 pos;
		v2f32 uvs;

		static constexpr v2i32 nb_components{ 3, 2 };
	};

	struct VertexRGBA
	{
		v3f32 pos;
		v4f32 color;
	};

	struct Vertex2D
	{
		v2f32 pos;
		v2f32 uvs;
	};

	struct Vertex2DRGBA
	{
		v2f32 pos;
		v4f32 color;
	};

}