#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Manage the generation of the world and the interactions between entities and said world
* ==============================================-
*/

#include "sys/types.hpp"

#include "chunkGrid.hpp"
#include "terrainGeneration.hpp"
#include "gfx/meshInstance.hpp"


namespace gfx
{
	struct RayCastResult;
	struct Camera;
	struct MeshInstance;

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

		Voxel get_voxel(types::voxel_pos voxel_p) const noexcept;
		bool set_voxel(types::voxel_pos voxel_p, Voxel new_voxel) noexcept;

		std::optional<RayCastResult> raycast(const types::pos& origin, const types::pos& dir, u64 max_length) noexcept;

		
		/// <summary>
		/// First draws chunk grids, then entities
		/// </summary>
		void draw(const Camera& camera) noexcept;


		ChunkGrid& get_chunkGrid() noexcept { return overworld; }
		const ChunkGrid& get_chunkGrid() const noexcept { return overworld; }


		static types::chunk_loc to_chunkLoc(const types::pos& pos) noexcept
		{
			return {
				static_cast<i64>(std::floor(pos.x / Chunk::g_size<f64>.x)),
				static_cast<i64>(std::floor(pos.y / Chunk::g_size<f64>.y)),
				static_cast<i64>(std::floor(pos.z / Chunk::g_size<f64>.z))
			};
		}

		static types::chunk_loc to_chunkLoc(const types::voxel_pos& pos) noexcept
		{
			return {
				integer_div_floor(pos.x, Chunk::g_size<i64>.x),
				integer_div_floor(pos.y, Chunk::g_size<i64>.y),
				integer_div_floor(pos.z, Chunk::g_size<i64>.z)
			};
		}

		static types::voxel_pos to_voxelPos(const types::pos& pos)
		{
			return static_cast<types::voxel_pos>(types::pos{ std::floor(pos.x), std::floor(pos.y), std::floor(pos.z) });
		}

		void loadModel(const filepath path);

	public:

		struct Debug
		{
			bool show_chunk_borders{};
			bool update_world{ true };
		} debug;


		gfx::TerrainGenContext/*<FastNoise::FractalFBm, FastNoise::Simplex>*/ terrain_context{};

		f64 gravity{ -32 };

		std::vector<gfx::MeshInstance> m_meshInstances;

	private:
		
		ChunkGrid overworld{};


		types::chunk_loc last_player_loc{}; // remove that when moved into the chunk grid class

	};
}