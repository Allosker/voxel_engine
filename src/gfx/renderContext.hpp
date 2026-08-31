#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Simple struct that is supposed to be passed down the chains of draw() functions for rendering
* ==============================================-
*/

#include "gfx/shader.hpp"
#include "gfx/texture.hpp"

namespace gfx
{
	
	struct RenderContext
	{
		const Shader*	sha{ nullptr };
		const Texture*	tex{ nullptr };
	};

}