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
#include <concepts>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>


// Special Macros

#define DELETE_COPY_INIT(Type) Type(const Type&) noexcept = delete; Type& operator=(Type&) = delete;
#define DEFAULT_COPY_INIT(Type) Type(const Type&) noexcept = default; Type& operator=(Type&) = default;

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

using v2i8 = glm::vec<2, i8>;
using v2u8 = glm::vec<2, u8>;

using v2i16 = glm::vec<2, i16>;
using v2u16 = glm::vec<2, u16>;

using v2i32 = glm::vec<2, i32>;
using v2u32 = glm::vec<2, u32>;

using v2i64 = glm::vec<2, i64>;
using v2u64 = glm::vec<2, u64>;


using v2f32 = glm::vec<2, f32>;
using v2f64 = glm::vec<2, f64>;



using v3i8 = glm::vec<3, i8>;
using v3u8 = glm::vec<3, u8>;

using v3i16 = glm::vec<3, i16>;
using v3u16 = glm::vec<3, u16>;
					   
using v3i32 = glm::vec<3, i32>;
using v3u32 = glm::vec<3, u32>;
					   
using v3i64 = glm::vec<3, i64>;
using v3u64 = glm::vec<3, u64>;
	  				

using v3f32 = glm::vec<3, f32>;
using v3f64 = glm::vec<3, f64>;



using v4i8 = glm::vec<4, i8>;
using v4u8 = glm::vec<4, u8>;

using v4i16 = glm::vec<4, i16>;
using v4u16 = glm::vec<4, u16>;

using v4i32 = glm::vec<4, i32>;
using v4u32 = glm::vec<4, u32>;

using v4i64 = glm::vec<4, i64>;
using v4u64 = glm::vec<4, u64>;


using v4f32 = glm::vec<4, f32>;
using v4f64 = glm::vec<4, f64>;


template <typename T>
inline void hash_combine(std::size_t& seed, const T& v)
{
	std::hash<T> hasher;
	// 0x9e3779b97f4a7c15 is a golden ratio constant to ensure bit distribution
	seed ^= hasher(v) + 0x9e3779b97f4a7c15 + (seed << 6) + (seed >> 2);
}

template <typename T, typename... Rest>
inline void hash_combine(std::size_t& seed, const T& v, Rest... rest)
{
	hash_combine(seed, v);
	hash_combine(seed, rest...);
}

namespace std
{
	template <typename U>
	struct hash<glm::vec<2, U>>
	{
		size_t operator ()(const glm::vec<2, U>& vec) const
		{
			std::size_t seed{};
			hash_combine(seed, vec.x, vec.y);

			return seed;
		}
	};

	template <typename U>
	struct hash<glm::vec<3, U>>
	{
		size_t operator ()(const glm::vec<3, U>& vec) const
		{
			std::size_t seed{};
			hash_combine(seed, vec.x, vec.y, vec.z);

			return seed;
		}
	};

	template <typename U>
	struct hash<glm::vec<4, U>>
	{
		size_t operator ()(const glm::vec<4, U>& vec) const
		{
			std::size_t seed{};
			hash_combine(seed, vec.x, vec.y, vec.z, vec.w);

			return seed;
		}
	};
}



using m3f32 = glm::mat<3, 3, f32>;
using m4f32 = glm::mat<4, 4, f32>;

using m4f64 = glm::mat<4, 4, f64>;

using qf64 = glm::qua<f64>;

using angle = f32;
using angle64 = f64;


// Small Utility Function to move into the Maths lib:

template<std::integral I>
inline I integer_div_floor(I a, I b) noexcept
{
	I res = a / b;
	I rem = a % b;

	// Correct for negative results with a non-zero remainder
	if (rem != 0 && ((a < 0) ^ (b < 0))) 
		res--;

	return res;
}


// Inner Types

namespace types
{

	using type_id = u16;
	constexpr inline type_id TypeIdNull{};

	using rgbs = u16;


	using chunk_loc = v3i64;
	/// <summary>
	/// Voxel absolute position within a chunk
	/// </summary>
	using voxel_loc = v3u16;

	/// <summary>
	/// World absolute position
	/// </summary>
	using voxel_pos = v3i64;
	/// <summary>
	/// World discrete position
	/// </summary>
	using pos = v3f64;


	template<typename T>
	struct Rect
	{
		T pos;
		T size;
	};

	/// <summary>
	/// 2D world discrete position
	/// </summary>
	using pos2d = v2f32;


}