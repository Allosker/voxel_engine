#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Set of functions for generating terrain based on perlin noise
* ==============================================-
*/

#include <FastNoise/FastNoise.h>

#include "sys_types.hpp"


namespace gfx
{
	/*template<typename T, typename U>*/
	struct TerrainGenContext
	{
		TerrainGenContext(f32 _octaves=3, f32 _gain=1, f32 _lacunarity=1, f32 _warp_amplitude=1, f32 _seed=0x0)
			: base{ FastNoise::New<FastNoise::Simplex>() }, noise{ FastNoise::New<FastNoise::FractalRidged>() }
		{
			noise->SetSource(base);
			noise->SetOctaveCount(octaves);

		}

		FastNoise::SmartNode<FastNoise::Simplex> base;
		FastNoise::SmartNode<FastNoise::FractalRidged> noise;
		f32 octaves{};
		f32 gain{};
		f32 lacunarity{};
		f32 warp_amplitude{};
		f32 seed{};
	};

	/*template<typename T, typename U>*/
	inline f32 continentalness(TerrainGenContext/*<T, U>*/& context, f32 z, f32 x) noexcept
	{
		f32 out{};
		auto r = context.noise->GenPositionArray2D(&out, 1, &z, &x, 0, 0, context.seed);

		return out * 0.5f + 0.5f;
	}

}