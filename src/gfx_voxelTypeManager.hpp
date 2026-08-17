#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Singleton accessible anywhere that provides the data corresponding to a a Voxel type ID.
* ==============================================-
*/

#include <vector>


#include "sys_types.hpp"

#include "gfx_voxelType.hpp"


namespace gfx
{

	class VoxelTypeManager
	{
	public:

		// = Public Access

		static const VoxelTypeManager& get() noexcept
		{
			static VoxelTypeManager instance{};

			return instance;
		}


		// = Getters

		const VoxelType& get_type(types::type_id id) const noexcept
		{
			return m_voxel_types[static_cast<size_t>(id)];
		}


	private:


		explicit VoxelTypeManager() noexcept
		{
			m_voxel_types.push_back({ .name{"air"}, .is_transparent{true}, .is_solid{ false } });
			m_voxel_types.push_back({ "stone" });
		}


		std::vector<VoxelType> m_voxel_types{};

	};

}