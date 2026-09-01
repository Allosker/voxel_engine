#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Simple container for a type_id, which allows for minimal data storing
* ==============================================-
*/

#include "gfx/voxelType.hpp"
#include "sys/types.hpp"


namespace gfx
{
	inline std::vector<v2f32> calculate_uvs(types::type_id id) noexcept
	{
		const auto& uvs = VoxelTypeManager::get().get_type(id).uvs;

		return
		{
			v2f32
			{ uvs.pos.x				 , uvs.pos.y },
			{ uvs.pos.x + uvs.size.x , uvs.pos.y },
			{ uvs.pos.x				 , uvs.pos.y + uvs.size.y },
			{ uvs.pos.x + uvs.size.x , uvs.pos.y },
			{ uvs.pos.x + uvs.size.x , uvs.pos.y + uvs.size.y },
			{ uvs.pos.x				 , uvs.pos.y + uvs.size.y }
		};
	}

	struct Voxel
	{

		types::type_id type_id{ types::TypeIdNull };

		operator bool() const
		{
			return type_id != types::TypeIdNull;
		}

		static constexpr std::array<std::array<v3f32, 6>, 6> g_model
		{
			// Position	
			
			/*Left*/
			std::array<v3f32, 6>
			{
				v3f32
				{ 1, 0, 1 },  /*Left-Down*/
				{ 1, 0, 0 },   /*Right-Down*/
				{ 1, 1, 1 },  /*Left-Up*/

				{ 1, 0, 0 },   /*Right-Down*/
				{ 1, 1, 0 },   /*Right-Up*/
				{ 1, 1, 1 },  /*Left-Up*/
			},

			// /*Right*/ 
			std::array<v3f32, 6>
			{
				v3f32
				{ 0, 0, 0 },  /*Right-Down*/
				{ 0, 0, 1 },   /*Left-Down*/
				{ 0, 1, 0 },  /*Right-Up*/

				{ 0, 0, 1 },   /*Left-Up*/
				{ 0, 1, 1 },  /*Right-Up*/
				{ 0, 1, 0 },   /*Left-Down*/
			},

		  
			/*Up*/	 
			std::array<v3f32, 6>
			{
				v3f32
				{ 1, 1, 1 },   /*Left-Down*/
				{ 1, 1, 0 },  /*Right-Down*/
				{ 0, 1, 1 },   /*Left-Up*/

				{ 1, 1, 0 },  /*Right-Down*/
				{ 0, 1, 0 },  /*Right-Up*/
				{ 0, 1, 1 },   /*Left-Up*/
			},
					  
			/*Down*/  
			std::array<v3f32, 6>
			{
				v3f32
				{ 1, 0, 0 },  /*Left-Down*/
				{ 1, 0, 1 },  /*Right-Down*/
				{ 0, 0, 0 },   /*Left-Up*/

				{ 1, 0, 1 },  /*Right-Down*/
				{ 0, 0, 1 },   /*Right-Up*/
				{ 0, 0, 0 },   /*Left-Up*/
			},


			/*Front*/
			std::array<v3f32, 6>
			{
				v3f32
				{ 0, 0, 1},  /*Left-Down*/
				{ 1, 0, 1 },  /*Right-Down*/
				{ 0, 1, 1 },  /*Left-Up*/

				{ 1, 0, 1 },  /*Right-Down*/
				{ 1, 1, 1 },  /*Right-Up*/
				{ 0, 1, 1 },  /*Left-Up*/
			},

			/*Back*/
			std::array<v3f32, 6>
			{
				v3f32
				{ 1, 0, 0 },  /*Left-Down*/
				{ 0, 0, 0 },  /*Right-Down*/
				{ 1, 1, 0 },  /*Left-Up*/

				{ 0, 0, 0 },  /*Right-Down*/
				{ 0, 1, 0 },  /*Right-Up*/
				{ 1, 1, 0 },  /*Left-Up*/
			},
			

		};

		static constexpr std::array<v2f32, 6> g_face_uvs
		{
			v2f32
			{ 0, 0 },
			{ 1, 0 },
			{ 0, 1 },
			{ 1, 0 },
			{ 1, 1 },
			{ 0, 1 }

		};

	};


}