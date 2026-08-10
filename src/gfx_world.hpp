#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Manage the generation of the world and the interactions between entities and said world
* ==============================================-
*/

#include "sys_types.hpp"

#include "gfx_chunkGrid.hpp"
#include "gfx_terrainGeneration.hpp"

namespace gfx
{
	class World
	{
	public:


		/// <summary>
		/// Allocate new chunks and calculates the terrain gen for them,
		/// create one new mesh every frame
		/// </summary>
		/// <param name="Player's location"></param>
		void update_grid(types::chunk_loc player_loc, bool reload = false) noexcept;

		void generate_world(const std::list<types::chunk_loc>& clocs) noexcept;
		
		/// <summary>
		/// First draws chunk grids, then entities
		/// </summary>
		void draw() const noexcept
		{
			overworld.draw();
		}


		ChunkGrid& get_chunkGrid() noexcept
		{
			return overworld;
		}


		static types::chunk_loc to_chunkLoc(const v3f32& pos) noexcept
		{
			return static_cast<types::chunk_loc>(pos / Chunk::g_size<f32>.x);
		}


	public:

		struct Debug
		{
			bool show_chunk_borders{};
		} debug;


		gfx::TerrainGenContext/*<FastNoise::FractalFBm, FastNoise::Simplex>*/ terrain_context{};

	private:
		
		ChunkGrid overworld{};


		types::chunk_loc last_player_loc{}; // remove that when moved into the chunk grid class

	};
}