#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Singleton accessible anywhere that provides the data corresponding to a a Voxel type ID.
* ==============================================-
*/

#include <string>
#include <vector>

#include "phy/hitboxAABB.hpp"
#include "sys/hash.hpp"
#include "sys/types.hpp"
#include <array>


namespace gfx
{

	/// <summary>
	/// All names are defaulted accounting for the average block
	/// </summary>
	struct VoxelType
	{
		/// <summary>
		/// Get the id back from the name
		/// </summary>
		std::string name{};

		phy::HitboxAABB hitbox;

		std::array<types::Rect<v2f32>, 6> uvs;


		bool is_transparent{ false };
		bool is_solid{ true };
		bool has_bounds{ false };

	};


	class VoxelTypeManager
	{
		using Uvs = types::Rect<v2f32>;

		/// <summary>
		/// All same
		/// </summary>
		/// <param name="main"></param>
		/// <returns></returns>
		static constexpr std::array<Uvs, 6> get_uvs(const Uvs& main) noexcept
		{	
			return { main, main, main, main, main, main };
		}

		/// <summary>
		/// Defines: up/down
		/// Defines: left/right/front/back
		/// </summary>
		/// <param name="y"></param>
		/// <param name="xz"></param>
		/// <returns></returns>
		static constexpr std::array<Uvs, 6> get_uvs(const Uvs& y, const Uvs& xz) noexcept
		{
			return { xz, xz, y, y, xz, xz };
		}

		/// <summary>
		/// Defines: up
		/// Defines: down
		/// Defines: left/right/front/back
		/// </summary>
		/// <param name="top"></param>
		/// <param name="down"></param>
		/// <param name="xz"></param>
		/// <returns></returns>
		static constexpr std::array<Uvs, 6> get_uvs(const Uvs& top, const Uvs& down, const Uvs& xz) noexcept
		{
			return { xz, xz, top, down, xz, xz };
		}

		/// <summary>
		/// Defines: up
		/// Defines: down
		/// Defines: left/right
		/// Defines: front/back
		/// </summary>
		/// <param name="top"></param>
		/// <param name="down"></param>
		/// <param name="x"></param>
		/// <param name="z"></param>
		/// <returns></returns>
		static constexpr std::array<Uvs, 6> get_uvs(const Uvs& top, const Uvs& down, const Uvs& x, const Uvs& z) noexcept
		{
			return { x, x, top, down, z, z };
		}



	public:

		

		static const VoxelTypeManager& get() noexcept
		{
			static VoxelTypeManager instance{};

			return instance;
		}


		/// <summary>
		/// Let'em crash
		/// </summary>
		const VoxelType& get_type(types::type_id id) const noexcept
		{
			return m_voxel_types[static_cast<size_t>(id)];
		}

		/// <summary>
		/// Let'em crash
		/// </summary>
		const VoxelType& get_type(const StringHash& name) const noexcept
		{
			return get_type(m_ids.at(name));
		}

		/// <summary>
		/// Let'em crash
		/// </summary>
		types::type_id get_id(const StringHash& name) const noexcept
		{
			return m_ids.at(name);
		}

		constexpr const StringHash& atlas_name() const noexcept { return m_atlas; }



	private:


		constexpr explicit VoxelTypeManager() noexcept
		{
			// Put the name on the right side to quickly know which type it is

			

			m_voxel_types.push_back(VoxelType{ .name{"air"},
				.hitbox{ types::pos{}, v3f64{ 0.5f} },
				.is_transparent{ true },
				.is_solid{ false } }
			);
			m_ids.emplace("air"_id, types::type_id_null);

			m_voxel_types.push_back(VoxelType{ .name{"stone"},
				.hitbox{ types::pos{}, v3f64{ 0.5f} },
				.uvs{ get_uvs(stone_uv) },
				.has_bounds{ true }
			});
			m_ids.emplace("stone"_id, 1);

			m_voxel_types.push_back(VoxelType{ .name{"dirt"},
				.hitbox{ types::pos{}, v3f64{ 0.5f} },
				.uvs{ get_uvs(dirt_uv) },
				.has_bounds{ true }
			});
			m_ids.emplace("dirt"_id, 2);

			m_voxel_types.push_back(VoxelType{ .name{"grass"},
				.hitbox{ types::pos{}, v3f64{ 0.5f} },
				.uvs{ get_uvs(grass_top_uv, dirt_uv, grass_side_uv) },
				.has_bounds{ true }
			});
			m_ids.emplace("grass"_id, 3);

		}

		StringHash m_atlas{ "textures/voxels/atlas"_id };
		std::vector<VoxelType> m_voxel_types{};
		std::unordered_map<StringHash, types::type_id> m_ids;

		/// <summary>
		/// In pixels 
		/// </summary>
		static constexpr f32 g_voxel_tex_size{ 33 };
		/// <summary>
		/// Allows for 100x100 textures be it: 10,000
		/// </summary>
		static constexpr f32 g_tex_size{ 3300 };

		static constexpr f32 g_ratio{ g_voxel_tex_size / g_tex_size };


		static constexpr v2f32 canonic_size{ g_ratio, g_ratio };


		static constexpr Uvs stone_uv{ {0, 0}, canonic_size };
		static constexpr Uvs dirt_uv{ { 1 * g_ratio, 0 }, canonic_size };
		static constexpr Uvs grass_top_uv{ { 2 * g_ratio, 0 }, canonic_size };
		static constexpr Uvs grass_side_uv{ { 3 * g_ratio, 0 }, canonic_size };


	};

}