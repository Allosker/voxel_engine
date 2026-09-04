#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	manages the loading/unloading of assets, which include: "shader/textures/.obj models/sounds"
* ==============================================-
*/

#include <print>
#include <string>
#include <unordered_map>


#include "gfx/font.hpp"
#include "gfx/mesh.hpp"
#include "gfx/model.hpp"
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
	std::unordered_map<std::string, gfx::Mesh>	    meshes;
	std::unordered_map<std::string, gfx::Model>	    models;
	std::unordered_map<std::string, gfx::Font>		fonts;


	void add_model(std::string id, const filepath& path);


private:

	AssetsManager() noexcept
	try
	{
		add_shaders();
		add_textures();
		add_models();
		add_fonts();
	}
	catch (const std::runtime_error& e)
	{
		std::println("{}", e.what());
	}


	void add_shaders()
	{
		shaders.emplace("shaders/world_chunks", gfx::Shader{ (filepath)ASSET_PATH"shader/world_chunks.vert", ASSET_PATH"shader/world_chunks.frag" });
		shaders.emplace("shaders/static_mesh", gfx::Shader{ (filepath)ASSET_PATH"shader/static_mesh.vert", ASSET_PATH"shader/static_mesh.frag" });
		shaders.emplace("shaders/twoD", gfx::Shader{ (filepath)ASSET_PATH"shader/twoD.vert", ASSET_PATH"shader/twoD.frag" });
		shaders.emplace("shaders/twoD_to_3D", gfx::Shader{ (filepath)ASSET_PATH"shader/twoD_to_3D.vert", ASSET_PATH"shader/twoD_to_3D.frag" });
		shaders.emplace("shaders/text", gfx::Shader{ (filepath)ASSET_PATH"shader/text.vert", ASSET_PATH"shader/text.frag" });
	}

	void add_textures()
	{
		textures.emplace("textures/voxels/atlas", gfx::Texture{ ASSET_PATH"textures/voxels/atlas.png" });

		textures.emplace("textures/gui/inventory/small", gfx::Texture{ ASSET_PATH"textures/gui/inventory/small.png" });
		textures.emplace("textures/gui/inventory/medium", gfx::Texture{ ASSET_PATH"textures/gui/inventory/medium.png" });
		textures.emplace("textures/gui/inventory/big", gfx::Texture{ ASSET_PATH"textures/gui/inventory/big.png" });
	}

	void add_models()
	{
		add_model("DamagedHelmet", ASSET_PATH "models/DamagedHelmet.glb");
	}

	void add_fonts()
	{
		fonts.emplace("fonts/november", gfx::Font{ ASSET_PATH"fonts/november/November.ttf" });
	}

};