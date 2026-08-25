#include "hitboxAABB.hpp"


namespace phy
{

	bool intersects(const HitboxAABB& a, const HitboxAABB& b) noexcept
	{
		return
			a.get_min().x < b.get_max().x && a.get_max().x > b.get_min().x &&

			a.get_min().y < b.get_max().y && a.get_max().y > b.get_min().y &&

			a.get_min().z < b.get_max().z && a.get_max().z > b.get_min().z;
	}

	v3f64 get_MTV(const HitboxAABB& a, const HitboxAABB& b) noexcept
	{
		const f64 right	{ a.get_max().x - b.get_min().x };
		const f64 down	{ a.get_max().y - b.get_min().y };
		const f64 back	{ a.get_max().z - b.get_min().z };

		const f64 left	{ b.get_max().x - a.get_min().x };
		const f64 up	{ b.get_max().y - a.get_min().y };
		const f64 front	{ b.get_max().z - a.get_min().z };

		v3f64 ret{};
		f64 bestDist{ -1 };


		if (left > 0 && (left < bestDist || bestDist < 0))
		{
			bestDist = left;
			ret = -v3f64{ left, 0, 0 };
		}

		if (right > 0 && (right < bestDist || bestDist < 0))
		{
			bestDist = right;
			ret = v3f64{ right, 0, 0 };
		}


		if (up > 0 && (up < bestDist || bestDist < 0))
		{
			bestDist = up;
			ret = -v3f64{ 0, up, 0 };
		}

		if (down > 0 && (down < bestDist || bestDist < 0))
		{
			bestDist = down;
			ret = v3f64{ 0, down, 0 };
		}


		if (front > 0 && (front < bestDist || bestDist < 0))
		{
			bestDist = front;
			ret = -v3f64{ 0, 0, front };
		}

		if (back > 0 && (back < bestDist || bestDist < 0))
		{
			bestDist = back;
			ret = v3f64{ 0, 0, back };
		}


		return ret;
	}

}