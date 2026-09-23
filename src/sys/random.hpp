#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Simple singleton class to provide random numbers
* ==============================================-
*/

#include "sys/types.hpp"
#include <memory>
#include <random>


class Random
{
public:


	static Random& get() noexcept
	{
		static Random instance{};
		return instance;
	}


	f64 next() noexcept
	{
		return dist->operator()(mse);
	}

	void reseed(u64 i) noexcept
	{
		mse.seed(i);
	}

	void set_range(f64 min, f64 max) noexcept
	{
		dist = std::make_unique<std::uniform_real_distribution<>>(min, max);
	}



private:

	Random()
		: dist{ std::make_unique<std::uniform_real_distribution<>>(-1, 1) }
	{
		std::random_device dev;

		mse.seed(dev());

	}

	std::mt19937 mse;

	std::unique_ptr<std::uniform_real_distribution<>> dist{};


};