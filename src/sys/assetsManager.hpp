#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	manages the loading/unloading of assets, which include: "shader/textures/.obj models/sounds"
* ==============================================-
*/

#include <unordered_map>
#include <string>

#include "sys/graphics.hpp"

#include "gfx/shader.hpp"
#include "gfx/texture.hpp"


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
		shaders.emplace("shader/world_chunks", gfx::Shader{ ASSET_PATH"shader/world_chunks.vert", ASSET_PATH"shader/world_chunks.frag" });
	}

	void add_textures() noexcept
	{
		textures.emplace("textures/voxels/stone", gfx::Texture{ ASSET_PATH"textures/voxels/stone.png" });
	}

};