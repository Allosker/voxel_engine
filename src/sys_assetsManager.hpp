#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	manages the loading/unloading of assets, which include: "shader/textures/.obj models/sounds"
* ==============================================-
*/

#include "sys_graphics.hpp"

#include "gfx_shader.hpp"
#include "gfx_texture.hpp"


// Loads all assets the first time "get()" is called.
// Note: this has to change when there are more assets
class AssetsManager
{
public:

	
	static AssetsManager& get() noexcept
	{
		static AssetsManager instance{};

		return instance;
	}


	gfx::Shader shader{ ASSET_PATH"shader/test.vert", ASSET_PATH"shader/test.frag" };

	gfx::Texture tex{ ASSET_PATH"textures/stone.png" };


private:


};