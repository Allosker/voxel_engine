#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Allocate/Deallocate chunks and their associated chunk meshes, a chunk may exist without an associated chunk mesh.
* ==============================================-
*/

#include <unordered_map>
#include <set>


#include "sys/types.hpp"

#include "chunk.hpp"
#include "chunkMesh.hpp"
#include "drawable.hpp"


namespace gfx
{
	class ChunkGrid : public Drawable
	{
	public:

		ChunkGrid() noexcept = default;


		/// <summary>
		/// Allocate new chunks into memory around a point in chunk coordinates, based on the rendering parameters.
		/// Chunk meshes aren't guaranteed to possess a chunk mesh.
		/// </summary>
		/// <param name="Location around which to build the chunks"></param>
		/// <returns>A list of the newly allocated chunks</returns>
		void allocate_chunks(const types::chunk_loc& min, const types::chunk_loc& max) noexcept;

		/// <summary>
		/// Deallocate/Allocate new chunks if the point location has changed.
		/// </summary>
		/// <param name="Point Location"></param>
		/// <returns>A list of the newly allocated chunks</returns>
		void manage_chunks(const types::chunk_loc& loc, bool force) noexcept;

		/// <summary>
		/// Update a chunk mesh for the provided chunk (via its corresponding chunkloc) if there exist one, allocate a new one otherwise
		/// </summary>
		/// <param name="Location of the chunk"></param>
		/// <returns>false if could not create chunk mesh</returns>
		bool update_cmesh(const types::chunk_loc& loc) noexcept;

		void dirty_cmesh(const types::chunk_loc& loc) noexcept;
		
		/// <summary>
		/// Allocate a new chunk mesh in sequence from the Queue
		/// - Allows for greater control over which chunk meshes have to be loaded first
		/// </summary>
		/// <returns>false if could not create chunk mesh</returns>
		void generate_pending_meshes(const types::chunk_loc& player_loc, f32 time_budget) noexcept;

		/// <summary>
		/// Deallocate chunks from memory around a point in chunk coordinates
		/// This also erases the chunk's associated chunk mesh, if it exists.
		/// </summary>
		/// <param name="location of the chunk"></param>
		/// <param name="Override for discarding all chunks regarless"></param>
		void deallocate_chunks(const types::chunk_loc& min, const types::chunk_loc& max) noexcept;

		void discard_all_chunks() noexcept;

		void draw(Renderer& renderer) override
		{
			for (auto& i : m_chunk_meshes)
				i.second.draw(renderer);
		}


		Voxel at(types::voxel_pos pos) const noexcept
		{
			auto cloc = pos / Chunk::g_size<i64>.x; 


			const Chunk* chunk = at_chunk(cloc);

			if (!chunk)
				return Voxel{ .type_id{0} };

			v3u16 vloc = static_cast<v3u16>(pos - chunk->get_position());

			if (vloc.x >= Chunk::g_size<u16>.x || vloc.y >= Chunk::g_size<u16>.y || vloc.z >= Chunk::g_size<u16>.z)
				return Voxel{ .type_id{0} };

			return chunk->at(vloc);
		}

		const Chunk* at_chunk(types::chunk_loc loc) const noexcept
		{
			auto l = m_chunks.find(loc);

			return l == m_chunks.end() ? nullptr : &l->second;
		}

		Chunk* at_chunk(types::chunk_loc loc) noexcept
		{
			auto l = m_chunks.find(loc);

			return l == m_chunks.end() ? nullptr : &l->second;
		}

		const ChunkMesh* at_chunkMesh(types::chunk_loc loc) const noexcept
		{
			auto l = m_chunk_meshes.find(loc);

			return l == m_chunk_meshes.end() ? nullptr : &l->second;
		}

		ChunkMesh* at_chunkMesh(types::chunk_loc loc) noexcept
		{
			auto l = m_chunk_meshes.find(loc);

			return l == m_chunk_meshes.end() ? nullptr : &l->second;
		}

		const std::unordered_map<types::chunk_loc, Chunk>& get_chunkMap() const noexcept { return m_chunks; }

		
	public:

		struct Parameters
		{
			u32 r_dist{ 2 }; // in chunks
			u32 r_height{ 2 };   // in chunks
		} parameters;

		std::unordered_set<types::chunk_loc> m_chunkGenQueue{};
		std::unordered_set<types::chunk_loc> m_chunkMeshQueue{};

	private:

		std::unordered_map<types::chunk_loc, Chunk> m_chunks{};
		std::unordered_map<types::chunk_loc, ChunkMesh> m_chunk_meshes{};

		types::chunk_loc m_min{};
		types::chunk_loc m_max{};

	};


}