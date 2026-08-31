#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Define a simple Mesh -- a GPU buffer possessing both a VAO and VBO
* This file depends on the content of "vertices.hpp"
* ==============================================-
*/


#include <vector>
#include <cassert>

#include "sys/graphics.hpp"


namespace gfx
{
	struct Mesh;
	struct Texture;

	struct Model
	{
		Mesh* mesh;
		std::vector<Texture*> textures;
	};


} // gfx