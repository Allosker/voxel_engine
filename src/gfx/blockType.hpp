#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Singleton accessible anywhere that provides the data corresponding to a a Voxel type ID.
* ==============================================-
*/

#include <string>
#include <vector>

#include "gfx/vertices.hpp"
#include "phy/hitboxAABB.hpp"
#include "sys/hash.hpp"
#include "sys/types.hpp"
#include "voxel.hpp"
#include <array>


namespace gfx
{

	/// <summary>
	/// All names are defaulted accounting for the average block
	/// </summary>
	struct BlockType
	{
		/// <summary>
		/// Get the id back from the name
		/// </summary>
		std::string name{};

		phy::HitboxAABB hitbox;

		std::vector<Vertex> model;


		bool is_transparent{ false };
		bool is_solid{ true };
		bool has_bounds{ false };

	};


	class BlockTypeManager
	{
		/// <summary>
		/// All same
		/// </summary>
		/// <param name="main"></param>
		/// <returns></returns>
		static constexpr std::array<types::Uvs, 6> get_uvs(const types::Uvs& main) noexcept
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
		static constexpr std::array<types::Uvs, 6> get_uvs(const types::Uvs& y, const types::Uvs& xz) noexcept
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
		static constexpr std::array<types::Uvs, 6> get_uvs(const types::Uvs& top, const types::Uvs& down, const types::Uvs& xz) noexcept
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
		static constexpr std::array<types::Uvs, 6> get_uvs(const types::Uvs& top, const types::Uvs& down, const types::Uvs& x, const types::Uvs& z) noexcept
		{
			return { x, x, top, down, z, z };
		}


		static inline std::vector<Vertex> get_simple_model(const std::array<types::Uvs, 6>& uvs) noexcept
		{
			std::vector<Vertex> ret;

			for (size_t i{}; i < uvs.size(); i++)
			{
				auto uv = uvs[i];

				uv.pos += 0.0001;
				uv.size -= 0.0001;

				ret.insert_range(
					ret.end(),
				std::vector<Vertex>
				{
					Vertex
					{ .pos{ Voxel::g_model[i][0] }, .uvs{ uv.pos.x				, uv.pos.y } },
					{ .pos{ Voxel::g_model[i][1] }, .uvs{ uv.pos.x + uv.size.x	, uv.pos.y } },
					{ .pos{ Voxel::g_model[i][2] }, .uvs{ uv.pos.x				, uv.pos.y + uv.size.y } },
					{ .pos{ Voxel::g_model[i][3] }, .uvs{ uv.pos.x + uv.size.x  , uv.pos.y } },
					{ .pos{ Voxel::g_model[i][4] }, .uvs{ uv.pos.x + uv.size.x  , uv.pos.y + uv.size.y } },
					{ .pos{ Voxel::g_model[i][5] }, .uvs{ uv.pos.x				, uv.pos.y + uv.size.y } },
				});
			}

			return ret;
		}



	public:



		static const BlockTypeManager& get() noexcept
		{
			static BlockTypeManager instance{};

			return instance;
		}


		/// <summary>
		/// Let'em crash
		/// </summary>
		const BlockType& get_type(types::type_id id) const noexcept
		{
			return m_voxel_types[static_cast<size_t>(id)];
		}

		/// <summary>
		/// Let'em crash
		/// </summary>
		const BlockType& get_type(const StringHash& name) const noexcept
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


		constexpr explicit BlockTypeManager() noexcept
		{
			// Put the name on the right side to quickly know which type it is
			m_voxel_types.push_back(BlockType{ .name{"air"},
				.hitbox{ types::pos{}, v3f64{ 0.5f} },
				.is_transparent{ true },
				.is_solid{ false } }
			);
			m_ids.emplace("air"_id, types::type_id_null);

			m_voxel_types.push_back(BlockType{ .name{"stone"},
				.hitbox{ types::pos{}, v3f64{ 0.5f} },
				.model{ get_simple_model(get_uvs(stone_uv)) },
				.has_bounds{ true }
			});
			m_ids.emplace("stone"_id, 1);

			m_voxel_types.push_back(BlockType{ .name{"dirt"},
				.hitbox{ types::pos{}, v3f64{ 0.5f} },
				.model{ get_simple_model(get_uvs(dirt_uv)) },
				.has_bounds{ true }
			});
			m_ids.emplace("dirt"_id, 2);

			m_voxel_types.push_back(BlockType{ .name{"grass"},
				.hitbox{ types::pos{}, v3f64{ 0.5f} },
				.model{ get_simple_model(get_uvs(grass_top_uv, dirt_uv, grass_side_uv)) },
				.has_bounds{ true }
			});
			m_ids.emplace("grass"_id, 3);

		}

		StringHash m_atlas{ "textures/voxels/atlas"_id };
		std::vector<BlockType> m_voxel_types{};
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


		static constexpr types::Uvs stone_uv{ {0, 0}, canonic_size };
		static constexpr types::Uvs dirt_uv{ { 1 * g_ratio, 0 }, canonic_size };
		static constexpr types::Uvs grass_top_uv{ { 2 * g_ratio, 0 }, canonic_size };
		static constexpr types::Uvs grass_side_uv{ { 3 * g_ratio, 0 }, canonic_size };


	};

}