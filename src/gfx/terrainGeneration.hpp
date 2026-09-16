#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Set of functions for generating terrain based on perlin noise
* ==============================================-
*/

#include <FastNoise/FastNoise.h>

#include "sys/types.hpp"


namespace gfx::terrain_gen
{
	/*template<typename T, typename U>*/
	struct Context
	{
		Context(f32 _octaves=3, f32 _gain=1, f32 _lacunarity=1, f32 _warp_amplitude=1, f32 _seed=0x0)
			: base{ FastNoise::New<FastNoise::Simplex>() }, noise{ FastNoise::New<FastNoise::FractalFBm>() }
		{
			noise->SetSource(base);
			noise->SetOctaveCount(octaves);
		}

		FastNoise::SmartNode<FastNoise::Simplex> base;
		FastNoise::SmartNode<FastNoise::FractalFBm> noise;
		f32 octaves{};
		f32 gain{};
		f32 lacunarity{};
		f32 warp_amplitude{};
		f32 seed{};
	};

	struct Data
	{
		Data(v2f32 size)
		{
			z_pos.resize(size.x * size.y);
			x_pos.resize(size.x * size.y);

			for (i32 z{}; z < size.x; z++)
				for (i32 x{}; x < size.y; x++)
				{
					z_pos[z * size.x + x] = static_cast<f32>(z);
					x_pos[z * size.y + x] = static_cast<f32>(x);
				}

			noise_data.resize(size.x * size.y);
		}

		std::vector<f32> noise_data;

		const std::vector<f32>& get_z_pos() const noexcept { return z_pos; }
		const std::vector<f32>& get_x_pos() const noexcept { return x_pos; }


	private:

		std::vector<f32> z_pos;
		std::vector<f32> x_pos;
	};

	/*template<typename T, typename U>*/
	inline f32 continentalness(Context/*<T, U>*/& context, Data& data, v2f32 chunk_pos) noexcept
	{
		f32 out{};
		auto r = context.noise->GenPositionArray2D(
			data.noise_data.data(),
			data.get_z_pos().size(),
			data.get_z_pos().data(),
			data.get_x_pos().data(),
			chunk_pos.x,
			chunk_pos.y,
			context.seed);

		return out * 0.5f + 0.5f;
	}

}