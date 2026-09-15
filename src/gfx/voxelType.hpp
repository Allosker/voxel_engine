#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Singleton accessible anywhere that provides the data corresponding to a a Voxel type ID.
* ==============================================-
*/

#include <string>
#include <vector>

#include "sys/hash.hpp"
#include "sys/types.hpp"


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

		types::Rect<v2f32> uvs{ };


		bool is_transparent{ false };
		bool is_solid{ true }; 
		bool has_bounds{ false };

	};
	 

	class VoxelTypeManager
	{
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
				.is_transparent{true}, 
				.is_solid{false}}
			);
			m_ids.emplace("air"_id, types::type_id_null);

			m_voxel_types.push_back(VoxelType{ .name{"stone"}, 
				.uvs{{0, 0}, {g_ratio, g_ratio}}, 
				.has_bounds{true}
			});
			m_ids.emplace("stone"_id, 1);

			m_voxel_types.push_back(VoxelType{ .name{"dirt"}, 
				.uvs{ { 1 * g_ratio, 0 }, { g_ratio, g_ratio } }, 
				.has_bounds{ true } 
			});
			m_ids.emplace("dirt"_id, 2);

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
	};

}