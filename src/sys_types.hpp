#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-	
*	Define all types widely used throughout the project
* - Maths Types
* - Inner Project Types
* ==============================================-	
*/

#include <cstdint>
#include <filesystem>

#include <mpml/mpml.hpp>


// Special Macros

#define DELETE_COPY_INIT(Type) Type(Type&) noexcept = delete; Type& operator=(Type&) = delete;
#define DEFAULT_COPY_INIT(Type) Type(Type&) noexcept = default; Type& operator=(Type&) = default;

#define DEFAULT_MOVE_INIT(Type) Type(Type&&) noexcept = default; Type& operator=(Type&&) = default;


// Basic Types

using i8 = std::int8_t;
using u8 = std::uint8_t;

using i16 = std::int16_t;
using u16 = std::uint16_t;

using i32 = std::int32_t;
using u32 = std::uint32_t;

using i64 = std::int64_t;
using u64 = std::uint64_t;


using f32 = float;
using f64 = double;


// STL Types

using filepath = std::filesystem::path;


// Maths Types

using v2i8 = mpml::Vector2<i8>;
using v2u8 = mpml::Vector2<u8>;

using v2i16 = mpml::Vector2<i16>;
using v2u16 = mpml::Vector2<u16>;

using v2i32 = mpml::Vector2<i32>;
using v2u32 = mpml::Vector2<u32>;

using v2i64 = mpml::Vector2<i64>;
using v2u64 = mpml::Vector2<u64>;


using v2f32 = mpml::Vector2<f32>;
using v2f64 = mpml::Vector2<f64>;



using v3i8 = mpml::Vector3<i8>;
using v3u8 = mpml::Vector3<u8>;
	   
using v3i16 = mpml::Vector3<i16>;
using v3u16 = mpml::Vector3<u16>;
	   
using v3i32 = mpml::Vector3<i32>;
using v3u32 = mpml::Vector3<u32>;
	   
using v3i64 = mpml::Vector3<i64>;
using v3u64 = mpml::Vector3<u64>;
	   
	   
using v3f32 = mpml::Vector3<f32>;
using v3f64 = mpml::Vector3<f64>;



using v4i8 = mpml::Vector4<i8>;
using v4u8 = mpml::Vector4<u8>;
	   
using v4i16 = mpml::Vector4<i16>;
using v4u16 = mpml::Vector4<u16>;
	   
using v4i32 = mpml::Vector4<i32>;
using v4u32 = mpml::Vector4<u32>;
	   
using v4i64 = mpml::Vector4<i64>;
using v4u64 = mpml::Vector4<u64>;
	   
	   
using v4f32 = mpml::Vector4<f32>;
using v4f64 = mpml::Vector4<f64>;


using m3f32 = mpml::Matrix3<f32>;
using m4f32 = mpml::Matrix4<f32>;


// Inner Types

namespace types
{

	// Voxels

	using type_id = u16;
	using rgbs = u16;
	

	// Chunks

	using chunk_loc = v3i64;
	using voxel_loc = v3u16;


	// world

	using voxel_point = v3i64;


	// 2D/3D

	template<typename T>
	struct Rect
	{
		T pos;
		T size;
	};


}