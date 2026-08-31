#include "phy/hitboxAABB_2D.hpp"

namespace phy
{

	bool intersects(const HitboxAABB2D& a, const HitboxAABB2D& b) noexcept
	{
		return
			a.get_min().x < b.get_max().x && a.get_max().x > b.get_min().x &&

			a.get_min().y < b.get_max().y && a.get_max().y > b.get_min().y;
	}

	bool intersects(const HitboxAABB2D& a, types::pos2d point) noexcept
	{
		return
			a.get_min().x < point.x && a.get_max().x > point.x &&

			a.get_min().y < point.y && a.get_max().y > point.y;
	}


	v2f32 get_MTV(const HitboxAABB2D& a, const HitboxAABB2D& b) noexcept
	{
		const f32 right{ a.get_max().x - b.get_min().x };
		const f32 down{ a.get_max().y - b.get_min().y };

		const f32 left{ b.get_max().x - a.get_min().x };
		const f32 up{ b.get_max().y - a.get_min().y };

		v2f32 ret{};
		f32 bestDist{ -1 };


		if (left > 0 && (left < bestDist || bestDist < 0))
		{
			bestDist = left;
			ret = -v2f32{ left, 0 };
		}

		if (right > 0 && (right < bestDist || bestDist < 0))
		{
			bestDist = right;
			ret = v2f32{ right, 0 };
		}


		if (up > 0 && (up < bestDist || bestDist < 0))
		{
			bestDist = up;
			ret = -v2f32{ 0, up };
		}

		if (down > 0 && (down < bestDist || bestDist < 0))
		{
			bestDist = down;
			ret = v2f32{ 0, down };
		}


		return ret;
	}


}