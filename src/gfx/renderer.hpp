#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
*/

#include <vector>

#include "sys/types.hpp"

namespace gfx
{
	class Mesh;
	struct Camera;
	class Material;

	struct DrawCommand
	{
		Mesh* mesh{};
		m4f32 transform{1};
		Material* material{};
	};

	class Renderer
	{
	public:

		void push_command(const DrawCommand& command)
		{
			m_commands.push_back(command);
		}

		void draw(const Camera& camera);


	private:

		std::vector<DrawCommand> m_commands;
	};
}