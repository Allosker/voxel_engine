#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
*/


namespace gfx
{
	class Renderer;

	class Drawable
	{
	public:

		~Drawable()
		{
		}

		virtual void draw(Renderer& renderer) = 0;
	};

}