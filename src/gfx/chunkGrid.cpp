#include "chunkGrid.hpp"


namespace gfx
{

	std::list<types::chunk_loc> ChunkGrid::allocate_chunks(types::chunk_loc cloc, bool override) noexcept
	{
		auto r_dist = static_cast<i64>(parameters.r_dist);
		auto r_height = static_cast<i64>(parameters.r_height);

		std::list<types::chunk_loc> ret{};

		v3i64 max
		{
			cloc.x + r_dist,
			cloc.y + r_height,
			cloc.z + r_dist
		};
		v3i64 min
		{
			cloc.x - r_dist,
			cloc.y - r_height,
			cloc.z - r_dist
		};

		for (i64 z{ min.z }; z <= max.z; z++)
		{
			for (i64 y{ min.y }; y <= max.y; y++)
			{
				for (i64 x{ min.x }; x <= max.x; x++)
				{
					if (m_chunks.try_emplace(types::chunk_loc{ x, y, z }, Chunk{ { x, y, z } }).second)
					{
						ret.emplace_back(x, y, z);
						//m_waiting_cmesh..({ x,y,z });
					}

					if (z == old_min.z || y == old_min.y || x == old_min.x || z == old_max.z || y == old_max.y || x == old_max.x)
					{
						const auto loc = types::chunk_loc{ x,y,z };

						if (const auto* const cmptr = at_chunkMesh(loc); cmptr && !cmptr->queued)
							add_cmesh(loc);
					}
				}
			}
		}

		old_min = min;
		old_max = max;

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

	bool ChunkGrid::update_cmesh(const types::chunk_loc& loc) noexcept
	{
		if (const auto* const cptr = at_chunk(loc))
		{
			if (cptr->isEmpty())
				return false;

			if (m_chunk_meshes.contains(loc))
			{
				auto& cm = m_chunk_meshes.at(loc);

				cm.update(*cptr, *this);
				cm.queued = false;
			}
			else
				m_chunk_meshes.emplace(std::pair<types::chunk_loc, ChunkMesh>{loc, ChunkMesh{*cptr, *this} });

			return true;
		}

		return false;
	}

	bool ChunkGrid::allocate_waiting_cmesh() noexcept
	{
		std::pair<types::chunk_loc, bool> elem{};

		bool successful{ true };

		do
		{
			if (m_waiting_cmesh.empty())
				return successful;

			elem = m_waiting_cmesh.front();
			m_waiting_cmesh.pop_front();

			successful = update_cmesh(elem.first) && successful;

		} while (elem.second);


		return successful;
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

		v3i64 max
		{
			cloc.x + r_dist,
			cloc.y + r_height,
			cloc.z + r_dist
		};
		v3i64 min
		{
			cloc.x - r_dist,
			cloc.y - r_height,
			cloc.z - r_dist
		};

		for (auto it = m_chunks.begin(); it != m_chunks.end();)
		{
			const auto loc = it->first;

			if ( 
				(loc.z > max.z || loc.z < min.z) ||
				(loc.y > max.y || loc.y < min.y) ||
				(loc.x > max.x || loc.x < min.x)
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