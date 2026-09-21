#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Manage the generation of the world and the interactions between entities and said world
* ==============================================-
*/

#include "sys/types.hpp"

#include "chunkGrid.hpp"
#include "drawable.hpp"
#include "gfx/meshInstance.hpp"
#include "terrainGeneration.hpp"

#include "gfx/world_item.hpp"
#include <utility>


namespace gfx
{
	struct RayCastResult;
	struct Camera;
	struct MeshInstance;

	class World : public Drawable
	{
	public:

		World()
			: terrain_data{ {Chunk::g_size<f32>.z, Chunk::g_size<f32>.x} }
		{
		}


		/// <summary>
		/// Allocate new chunks and calculates the terrain gen for them,
		/// create one new mesh every frame
		/// </summary>
		/// <param name="Player's location"></param>
		void update_grid(types::chunk_loc player_loc, bool reload = false) noexcept;

		void generate_pending_chunks(const types::chunk_loc& player_loc) noexcept;
		void generate_chunk(const types::chunk_loc& loc) noexcept;

		Voxel get_voxel(types::voxel_pos voxel_p) const noexcept;
		bool set_voxel(types::voxel_pos voxel_p, Voxel new_voxel) noexcept;

		std::optional<RayCastResult> raycast(const types::pos& origin, const types::pos& dir, u64 max_length) noexcept;


		/// <summary>
		/// First draws chunk grids, then entities
		/// </summary>
		void draw(Renderer& renderer);


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

		void load_model(const filepath path);

		/// <summary>
		/// Considers the loc to be valid
		/// </summary>
		/// <param name="loc"></param>
		/// <returns></returns>
		const std::unordered_map<types::chunk_loc, std::vector<WorldItem>>& get_entities(const types::chunk_loc& loc) const noexcept { return m_entities; }

		/// <summary>
		/// Considers the loc to be valid
		/// </summary>
		/// <param name="loc"></param>
		/// <param name="wi"></param>
		void add_entity(const types::chunk_loc& loc, WorldItem&& wi) noexcept
		{
			m_entities.at(loc).push_back(std::move(wi));
		}
		  
		/// <summary>
		/// Loop through all entities and removes the ones responding true to the lambda expression
		/// <para> Note: the lambda expression accepts the entity being processed</para>
		/// <para> Note: considers loc to be valid</para>
		/// </summary>
		/// <param name="lambda"></param>
		template<typename LAMBDA>
		void remove_entities_if(const types::chunk_loc& loc, LAMBDA lambda) noexcept
		{
			auto& entities = m_entities.at(loc);
			std::vector<WorldItem>::iterator i{};
			while (i != entities.end())
			{
				if (lambda(*i))
				{
					i = entities.erase(i);
				}
				else
					i++;
			}
		}


	public:

		struct Debug
		{
			bool show_chunk_borders{};
			bool update_world{ true };
		} debug;

		static constexpr f32 time_budget{ 4.f / 1000.f };

		gfx::terrain_gen::Context terrain_context{};
		gfx::terrain_gen::Data terrain_data;

		f64 gravity{ -32 };

		std::vector<gfx::MeshInstance> m_meshInstances;


	private:

		ChunkGrid overworld{};

		std::unordered_map<types::chunk_loc, std::vector<WorldItem>> m_entities;

		types::chunk_loc last_player_loc{}; // remove that when moved into the chunk grid class

		i32 m_entity_assign_count{};


	};
}