#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Define the Chunk class, simple container for voxels which can be placed contiguously next to other chunks.
* A chunk class can consume up to 32'678*sizeof(Voxel)bytes of RAM.
* ==============================================-
*/

#include <array> 

#include "sys_graphics.hpp"
#include "sys_types.hpp"

#include "gfx_voxel.hpp"


namespace gfx
{


	class Chunk
	{
	public:


		// = Init

		Chunk(types::chunk_loc location) noexcept;

		DEFAULT_MOVE_INIT(Chunk);
		DELETE_COPY_INIT(Chunk);


		~Chunk() noexcept = default;


		bool isEmpty() const noexcept { return m_empty; }
		

		Voxel& at(types::voxel_loc loc) noexcept { return m_empty ? m_voxels[0] : m_voxels[loc.x + loc.y * g_size<u16>.y + loc.z * g_size<u16>.z * g_size<u16>.z]; }

		const Voxel& at(types::voxel_loc loc) const noexcept { return m_empty ? m_voxels[0] : m_voxels[loc.x + loc.y * g_size<u16>.y + loc.z * g_size<u16>.z * g_size<u16>.z]; }

		v3i64 get_position() const noexcept { return static_cast<v3i64>(m_loc) * g_size<i64>.x; }

		types::chunk_loc get_location() const noexcept { return m_loc; }


		void set_voxel_at(types::voxel_loc loc, Voxel new_voxel) noexcept;

		void set_empty() noexcept;


	public:

		template<typename T>
		static constexpr mpml::Vector3<T> g_size{ 32 };
		static constexpr auto g_absolute_size{ g_size<size_t>.x * g_size<size_t>.y * g_size<size_t>.z };
		
		template<typename T>
		static inline constexpr std::array<mpml::Vector3<T>, 6> dirs
		{
			mpml::Vector3<T>
			{ 1,  0,  0 },
			{ -1, 0,  0 },
			{ 0,  1,  0 },
			{ 0, -1,  0 },
			{ 0,  0,  1 },
			{ 0,  0, -1 }
		};

		template<typename T>
		static inline constexpr std::array<mpml::Vector3<T>, 6> surroundings
		{
			mpml::Vector3<T>
			{ 1,  0,  0 },

			{ -1, 0,  0 },
			{ 0,  1,  0 },
			{ 0, -1,  0 },
			{ 0,  0,  1 },
			{ 0,  0, -1 }
		};


	private:

		std::vector<Voxel> m_voxels{};

		types::chunk_loc m_loc{};
		bool m_empty{ false };

	};



}