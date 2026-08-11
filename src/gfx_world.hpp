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

		bool set_voxel(types::voxel_point voxel_p, Voxel new_voxel) noexcept;
		
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
			return {
				static_cast<i64>(std::floor(pos.x / Chunk::g_size<f32>.x)),
				static_cast<i64>(std::floor(pos.y / Chunk::g_size<f32>.y)),
				static_cast<i64>(std::floor(pos.z / Chunk::g_size<f32>.z))
			};
		}

		static types::chunk_loc to_chunkLoc(const types::voxel_point& pos) noexcept
		{
			return {
				pos.x / Chunk::g_size<i64>.x,
				pos.y / Chunk::g_size<i64>.y,
				pos.z / Chunk::g_size<i64>.z
			};

			// Fix that, it does not work correctly since -1/32 in integer division gives -0 which is zero, figure this out when you are not tired!
		}


	public:

		struct Debug
		{
			bool show_chunk_borders{};
			bool update_world{ true };
		} debug;


		gfx::TerrainGenContext/*<FastNoise::FractalFBm, FastNoise::Simplex>*/ terrain_context{};

	private:
		
		ChunkGrid overworld{};


		types::chunk_loc last_player_loc{}; // remove that when moved into the chunk grid class

	};
}