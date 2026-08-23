#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	This class allocates hardcoded data once and only once that is generalised, meaning it can be queried and adapted based on the context.
* ==============================================-
*/

#include <string>


#include "texture.hpp"


namespace gfx
{
	/// <summary>
	/// All names are defaulted accounting for the average block
	/// </summary>
	struct VoxelType
	{
		std::string name{};

		bool is_transparent{ false };
		bool is_solid{ true };
		bool has_bounds{ false };

	};

}