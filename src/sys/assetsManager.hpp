#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	manages the loading/unloading of assets, which include: "shader/textures/.obj models/sounds"
* ==============================================-
*/

#include <string>
#include <unordered_map>


#include "gfx/shader.hpp"
#include "gfx/texture.hpp"
#include "sys/types.hpp"


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

	std::unordered_map<std::string, gfx::Texture>	textures;
	std::unordered_map<std::string, gfx::Shader>	shaders;


private:

	AssetsManager() noexcept
	{
		add_shaders();
		add_textures();
	}


	void add_shaders() noexcept
	{
		shaders.emplace("shaders/world_chunks", gfx::Shader{ (filepath)ASSET_PATH"shader/world_chunks.vert", ASSET_PATH"shader/world_chunks.frag" });
		shaders.emplace("shaders/twoD", gfx::Shader{ (filepath)ASSET_PATH"shader/twoD.vert", ASSET_PATH"shader/twoD.frag" });
		shaders.emplace("shaders/twoD_to_3D", gfx::Shader{ (filepath)ASSET_PATH"shader/twoD_to_3D.vert", ASSET_PATH"shader/twoD_to_3D.frag" });
	}

	void add_textures() noexcept
	{
		textures.emplace("textures/voxels/stone", gfx::Texture{ ASSET_PATH"textures/voxels/stone.png" });
		textures.emplace("textures/gui/inventory/small", gfx::Texture{ ASSET_PATH"textures/gui/inventory/small.png" });
		textures.emplace("textures/gui/inventory/medium", gfx::Texture{ ASSET_PATH"textures/gui/inventory/medium.png" });
		textures.emplace("textures/gui/inventory/big", gfx::Texture{ ASSET_PATH"textures/gui/inventory/big.png" });
	}

};