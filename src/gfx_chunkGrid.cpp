#include "gfx_chunkGrid.hpp"


namespace gfx
{

	std::list<types::chunk_loc> ChunkGrid::allocate_chunks(types::chunk_loc cloc, bool override) noexcept
	{
		auto r_dist = static_cast<i64>(parameters.r_dist);
		auto r_height = static_cast<i64>(parameters.r_height);

		std::list<types::chunk_loc> ret{};

		for (i64 z{ cloc.z - r_dist }; z < cloc.z + r_dist; z++)
		{
			for (i64 y{ cloc.y - r_height }; y < cloc.y + r_height; y++)
			{
				for (i64 x{ cloc.x - r_dist }; x < cloc.x + r_dist; x++)
				{
					if (m_chunks.try_emplace(types::chunk_loc{ x, y, z }, Chunk{ { x, y, z } }).second)
					{
						ret.emplace_back(x, y, z);
					}

				}
			}
		}

		return ret;
	}

	std::list<types::chunk_loc> ChunkGrid::manage_chunks(types::chunk_loc loc, bool override) noexcept
	{
		if (!override && loc == last_loc)
			return {};
		last_loc = loc;

		deallocate_chunks(loc, override);
		return allocate_chunks(loc, override);
	}

	bool ChunkGrid::allocate_cmesh(const types::chunk_loc& loc) noexcept
	{
		if (auto* cptr = at_chunk(loc))
		{
			if (cptr->isEmpty())
				return false;

			return m_chunk_meshes.try_emplace(loc, *cptr, *this).second;
		}

		return false;
	}

	bool ChunkGrid::allocate_waiting_cmesh() noexcept
	{
		if (m_waiting_cmesh.empty())
			return false;

		types::chunk_loc loc{ m_waiting_cmesh.front() };
		m_waiting_cmesh.pop();

		return allocate_cmesh(loc);
	}

	void ChunkGrid::deallocate_chunks(types::chunk_loc cloc, bool override) noexcept
	{
		if (override)
		{
			m_chunks.clear();
			m_chunk_meshes.clear();
			return;
		}

		auto r_dist = static_cast<i64>(parameters.r_dist);
		auto r_height = static_cast<i64>(parameters.r_dist);

		v3i64 u_bounds
		{
			cloc.x + r_dist,
			cloc.y + r_height,
			cloc.z + r_dist
		};
		v3i64 l_bounds
		{
			cloc.x - r_dist,
			cloc.y - r_height,
			cloc.z - r_dist
		};

		for (auto it = m_chunks.begin(); it != m_chunks.end();)
		{
			const auto loc = it->first;

			if ( 
				(loc.z > u_bounds.z || loc.z < l_bounds.z) ||
				(loc.y > u_bounds.y || loc.y < l_bounds.y) ||
				(loc.x > u_bounds.x || loc.x < l_bounds.x)
				)
			{
				it = m_chunks.erase(it);
				m_chunk_meshes.erase(loc);
			}
			else
				it++;
		}
	}

}