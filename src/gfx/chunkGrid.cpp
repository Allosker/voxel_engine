#include "chunkGrid.hpp"

#include <print>

namespace gfx
{

	void ChunkGrid::allocate_chunks(const types::chunk_loc& min, const types::chunk_loc& max) noexcept
	{
		std::list<types::chunk_loc> ret{};

		for (i64 z{ min.z }; z <= max.z; z++)
		{
			for (i64 y{ min.y }; y <= max.y; y++)
			{
				for (i64 x{ min.x }; x <= max.x; x++)
				{
					const auto loc = types::chunk_loc{ x,y,z };

					if (m_chunks.try_emplace(loc, Chunk{ loc }).second)
					{
						m_chunkGenQueue.insert(loc);
					}
				}
			}
		}
	}

	void ChunkGrid::manage_chunks(const types::chunk_loc& loc, bool force) noexcept
	{
		const auto r_dist = static_cast<i64>(parameters.r_dist);
		const auto r_height = static_cast<i64>(parameters.r_height);

		const v3i64 max
		{
			loc.x + r_dist,
			loc.y + r_height,
			loc.z + r_dist
		};
		const v3i64 min
		{
			loc.x - r_dist,
			loc.y - r_height,
			loc.z - r_dist
		};


		deallocate_chunks(min, max);
		allocate_chunks(min, max);
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

	void ChunkGrid::dirty_cmesh(const types::chunk_loc& loc) noexcept
	{
		m_chunkMeshQueue.insert(loc);
	}

	void ChunkGrid::generatePendingMeshes(const types::chunk_loc& player_loc) noexcept
	{
		const auto timeBudget = 4 / 1000.f;
		const auto start = std::chrono::steady_clock::now();

		int generatedCount{};

		while (!m_chunkMeshQueue.empty())
		{
			generatedCount++;

			const auto chunkStart = std::chrono::steady_clock::now();

			const auto squareDist = [](const types::chunk_loc & a, const types::chunk_loc & b)
			{
				return (a.x - b.x) * (a.x - b.x) + (a.z - b.z) * (a.z - b.z);
			};

			auto closest = std::min_element(m_chunkMeshQueue.begin(), m_chunkMeshQueue.end(),
				[&](const types::chunk_loc& a, const types::chunk_loc& b) {
					return squareDist(a, player_loc) < squareDist(b, player_loc);
				}
			);

			const auto elem = *closest;
			m_chunkMeshQueue.erase(closest);

			update_cmesh(elem);

			const auto end = std::chrono::steady_clock::now();

			const auto chunkTime = std::chrono::duration<float>{end - chunkStart}.count();
			const auto totalTime = std::chrono::duration<float>{end - start}.count();

			if (totalTime + chunkTime > timeBudget)
			{
				break;
			}
		}

		if (generatedCount)
		{
			const auto end = std::chrono::steady_clock::now();
			const auto totalTime = std::chrono::duration<float>{end - start}.count();
		}

	}

	void ChunkGrid::deallocate_chunks(const types::chunk_loc& min, const types::chunk_loc& max) noexcept
	{
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

	void ChunkGrid::discard_all_chunks() noexcept
	{
		m_chunks.clear();
		m_chunk_meshes.clear();
		return;
	}

}