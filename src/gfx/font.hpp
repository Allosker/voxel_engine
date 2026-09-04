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

		Font(Font&& other) noexcept;
		Font& operator=(Font&& other) noexcept;

		~Font() noexcept;


		const Character& getCharacter(u8 c) const noexcept { return m_characters.at(c); }



		void update_resolution(const filepath& path, u8 btmp_size) noexcept
		{
			glDeleteTextures(1, &m_texture_id);
			m_characters.clear();
			m_btmp_size = btmp_size;
			load(path);
		}
	

		void load(const filepath& path) noexcept;

		void bind() const noexcept { glBindTexture(GL_TEXTURE_2D, m_texture_id); }


	private:

		// = Actors

		Image create_bitmap(const filepath& path) noexcept;
		

	private:

		std::map<u8, Character> m_characters{};

		GLuint	m_texture_id{};
		u8		m_btmp_size{};


		static constexpr u32 nb_glyphs{ 128 };
		static constexpr u32 size_bitmap{ 512 };
		static constexpr u32 g_padding_pixels{ 2 };

	};

}