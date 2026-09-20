#pragma once // text.hpp
// MIT
// Allosker ------------------------------
// =========+
// This class manages a text entity that can be transformed
// ---------------------------------------

#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "sys/graphics.hpp"
#include "sys/types.hpp"

#include "gfx/image.hpp"
#include "gfx/texture.hpp"


namespace gfx
{

	struct Character
	{
		v2u32	pos{};
		v2u32	size{};
		v2i32	bearing{};
		u32		advance{};
		u32		kerning{};
	};

	class Font
	{
	public:

		Font(const filepath& path, u8 btmp_size = 48) noexcept;

		DELETE_COPY_INIT(Font);
		DEFAULT_MOVE_INIT(Font);

		const Character& get_character(u8 c) const noexcept { return m_characters.at(c); }


		void update_resolution(const filepath& path, u8 btmp_size) noexcept
		{
			m_tex.unload();
			m_characters.clear();
			m_btmp_size = btmp_size;
			load(path);
		}
	

		void load(const filepath& path) noexcept;

		Texture& get_tex() noexcept { return m_tex; }
		const Texture& get_tex() const noexcept { return m_tex; }


	private:

		Image create_bitmap(const filepath& path) noexcept;
		

	private:

		std::map<u8, Character> m_characters{};

		Texture m_tex;
		u8		m_btmp_size{};


		static constexpr u32 nb_glyphs{ 128 };
		static constexpr u32 size_bitmap{ 512 };
		static constexpr u32 g_padding_pixels{ 2 };

	};

}