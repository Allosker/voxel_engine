#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Allocate/Deallocate chunks and their associated chunk meshes, a chunk may exist without an associated chunk mesh.
* ==============================================-
*/

#include <unordered_map>
#include <list>
#include <deque>


#include "sys/types.hpp"

#include "chunk.hpp"
#include "chunkMesh.hpp"


namespace gfx
{
	class ChunkGrid
	{
	public:

		ChunkGrid() noexcept = default;


		/// <summary>
		/// Allocate new chunks into memory around a point in chunk coordinates, based on the rendering parameters.
		/// Chunk meshes aren't guaranteed to possess a chunk mesh.
		/// </summary>
		/// <param name="Location around which to build the chunks"></param>
		/// <param name="Override for rebuilding all chunks regardless"></param>
		/// <returns>A list of the newly allocated chunks</returns>
		std::list<types::chunk_loc> allocate_chunks(types::chunk_loc cloc, bool override = false) noexcept;

		/// <summary>
		/// Deallocate/Allocate new chunks if the point location has changed.
		/// </summary>
		/// <param name="Point Location"></param>
		/// <param name="Override for discarding and then rebuilding all chunks regardless"></param>
		/// <returns>A list of the newly allocated chunks</returns>
		std::list<types::chunk_loc> manage_chunks(types::chunk_loc loc, bool override = false) noexcept;

		/// <summary>
		/// Update a chunk mesh for the provided chunk (via its corresponding chunkloc) if there exist one, allocate a new one otherwise
		/// </summary>
		/// <param name="Location of the chunk"></param>
		/// <returns>false if could not create chunk mesh</returns>
		bool update_cmesh(const types::chunk_loc& loc) noexcept;
		
		/// <summary>
		/// Allocate a new chunk mesh in sequence from the Queue
		/// - Allows for greater control over which chunk meshes have to be loaded first
		/// </summary>
		/// <returns>false if could not create chunk mesh</returns>
		bool allocate_waiting_cmesh() noexcept;

		/// <summary>
		/// Deallocate chunks from memory around a point in chunk coordinates
		/// This also erases the chunk's associated chunk mesh, if it exists.
		/// </summary>
		/// <param name="location of the chunk"></param>
		/// <param name="Override for discarding all chunks regarless"></param>
		void deallocate_chunks(types::chunk_loc cloc, bool override = false) noexcept;

		/// <summary>
		/// Add all the elements to the back of the deque, no matter whether the meshes already exist
		/// </summary>
		/// /// <param name="Location of the chunk"></param>
		void add_cmeshes(const std::list<types::chunk_loc>& cloc) noexcept
		{
			for (const auto& i : cloc)
				m_waiting_cmesh.push_back({ i, false });
		}

		/// <summary>
		/// Add the element to the back of the deque
		/// </summary>
		/// /// <param name="Location of the chunk"></param>
		void add_cmesh(const types::chunk_loc& cloc, bool keep_updating_whereupon = false) noexcept
		{
			m_chunk_meshes.at(cloc).queued = true;
			m_waiting_cmesh.push_back({ cloc, keep_updating_whereupon });
		}

		void add_priority_cmesh(const types::chunk_loc& cloc, bool keep_updating_whereupon = false) noexcept
		{
			m_chunk_meshes.at(cloc).queued = true;
			m_waiting_cmesh.push_front({cloc, keep_updating_whereupon});
		}


		void draw() const noexcept
		{
			glEnable(GL_CULL_FACE);

			for (const auto& i : m_chunk_meshes)
				i.second.draw();

			glDisable(GL_CULL_FACE);
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


	private:

		std::unordered_map<types::chunk_loc, Chunk> m_chunks{};
		std::unordered_map<types::chunk_loc, ChunkMesh> m_chunk_meshes{};

		/// <summary>
		/// The first element represents the location of the chunk mesh to be created/updated
		/// <para>The second element represents whether we should stop updating chunk meshes whereupon </para>
		/// </summary>
		std::deque<std::pair<types::chunk_loc, bool>> m_waiting_cmesh{};

		types::chunk_loc old_min{};
		types::chunk_loc old_max{};

		// Old location around which to allocate the new chunks
		types::chunk_loc last_loc{};
	};


}