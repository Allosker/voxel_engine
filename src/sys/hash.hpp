#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================
*/

#include <compare>
#include <string_view>

#include "types.hpp"

using Hash32 = uint32_t;
using Hash64 = uint32_t;

struct StringHash
{
public:
	constexpr StringHash(std::string_view str)
		: m_hash(compute(str))
	{}

	template<size_t N>
	constexpr StringHash(const char(&str)[N])
		: StringHash(std::string_view(str, N - 1))

	{}
	constexpr StringHash(const std::string& str)
		: m_hash(compute(str))
	{}

	constexpr operator uint32_t() const
	{
		return m_hash;
	}

	constexpr uint32_t getHash() const
	{
		return m_hash;
	}

	auto operator<=>(const StringHash&) const = default;

private:
	static constexpr Hash32 compute(std::string_view str)
	{
		constexpr uint32_t FNV_PRIME = 16777619u;
		constexpr uint32_t FNV_OFFSET_BASIS = 2166136261u;

		uint32_t hash = FNV_OFFSET_BASIS;
		for (char c : str)
		{
			hash ^= static_cast<uint32_t>(c);
			hash *= FNV_PRIME;
		}

		return hash;
	}

	uint32_t m_hash;
};

constexpr StringHash operator""_id(const char* str, size_t size)
{
	return StringHash(std::string_view(str, size));
}

namespace std
{
	template<>
	struct hash<StringHash>
	{
		size_t operator()(const StringHash& hash) const noexcept
		{
			return static_cast<size_t>(hash.getHash());
		}
	};
}