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
#include "gfx/mesh.hpp"
#include "gfx/model.hpp"
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
	std::unordered_map<std::string, gfx::Mesh>	    meshes;
	std::unordered_map<std::string, gfx::Model>	    models;


	void add_model(std::string id, const filepath& path);

private:

	AssetsManager() noexcept
	{
		add_shaders();
		add_textures();
		add_models();
	}


	void add_shaders() noexcept
	{
		shaders.emplace("shaders/world_chunks", gfx::Shader{ (filepath)ASSET_PATH"shader/world_chunks.vert", ASSET_PATH"shader/world_chunks.frag" });
		shaders.emplace("shaders/static_mesh", gfx::Shader{ (filepath)ASSET_PATH"shader/static_mesh.vert", ASSET_PATH"shader/static_mesh.frag" });
		shaders.emplace("shaders/twoD", gfx::Shader{ (filepath)ASSET_PATH"shader/twoD.vert", ASSET_PATH"shader/twoD.frag" });
	}

	void add_textures() noexcept
	{
		textures.emplace("textures/voxels/stone", gfx::Texture{ ASSET_PATH"textures/voxels/stone.png" });
		textures.emplace("textures/gui/inventory/small", gfx::Texture{ ASSET_PATH"textures/gui/inventory/small.png" });
		textures.emplace("textures/gui/inventory/medium", gfx::Texture{ ASSET_PATH"textures/gui/inventory/medium.png" });
		textures.emplace("textures/gui/inventory/big", gfx::Texture{ ASSET_PATH"textures/gui/inventory/big.png" });
	}

	void add_models() noexcept
	{
		add_model("DamagedHelmet", ASSET_PATH "models/DamagedHelmet.glb");
	}
};