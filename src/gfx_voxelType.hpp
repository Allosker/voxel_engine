#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	This class allocates hardcoded data once and only once that is generalised, meaning it can be queried and adapted based on the context.
* ==============================================-
*/

#include <string>


#include "gfx_texture.hpp"


namespace gfx
{

	struct VoxelType
	{
		std::string name{};

		bool is_transparent{};
		bool is_traversable{};

	};

}