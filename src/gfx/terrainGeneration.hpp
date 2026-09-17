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
		std::vector<f32> noise_data;
	};

	inline void generate(Context& context, Data& data, v3f32 chunk_pos) noexcept
	{
		data.noise_data.resize(Chunk::g_size<f32>.x * Chunk::g_size<f32>.z);
		auto minMax = context.noise->GenUniformGrid2D(
			data.noise_data.data(),
			chunk_pos.x, chunk_pos.z,
			Chunk::g_size<f32>.x, Chunk::g_size<f32>.z,
			1.0f, 1.0f,
			context.seed
		);
	}

}