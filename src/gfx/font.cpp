#include "font.hpp"


namespace gfx
{

	Font::Font(const filepath& path, u8 btmp_size) noexcept
		: m_btmp_size{ btmp_size }
	{
		load(path);
	}

	Font::Font(Font&& other) noexcept
		: m_texture_id{ other.m_texture_id }, m_characters{ other.m_characters }, m_btmp_size{ other.m_btmp_size }
	{
		other.m_texture_id = {};
		std::map<u8, Character>{}.swap(other.m_characters);
		other.m_btmp_size = {};
	}

	Font& Font::operator=(Font&& other) noexcept
	{
		if (this == &other) return *this;

		m_texture_id = other.m_texture_id;
		m_characters = other.m_characters;
		m_btmp_size = other.m_btmp_size;

		other.m_texture_id = {};
		std::map<u8, Character>{}.swap(other.m_characters);
		other.m_btmp_size = {};

		return *this;
	}

	Font::~Font() noexcept
	{
		glDeleteTextures(1, &m_texture_id);
	}

	void Font::load(const filepath& path) noexcept
	{
		// Disable Default Texture Alignment
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		Image temp{ std::move(create_bitmap(path)) };

		glGenTextures(1, &m_texture_id);
		glBindTexture(GL_TEXTURE_2D, m_texture_id);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			temp.getSize().x,
			temp.getSize().y,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			temp.data()
		);


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	/*private*/ Image Font::create_bitmap(const filepath& path) noexcept
	{
		FT_Library ft;
		if (FT_Init_FreeType(&ft))
			throw std::runtime_error("ERROR::FREETYPE: Could not init FreeType Library");

		FT_Face face;
		if (FT_New_Face(ft, path.string().c_str(), 0, &face))
			throw std::runtime_error("ERROR::FREETYPE: Failed to load");

		FT_Set_Pixel_Sizes(face, 0, m_btmp_size);

		Image bitmap{ v2u32{ size_bitmap }, GL_RED };


		v2u32 pos_btmp{};
		u32 highest{};
		u8 previous{};
		for (u8 c{}; c < nb_glyphs; c++)
		{
			// Load one char at a time
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
				continue;

			Image glyph{
				{ face->glyph->bitmap.width, face->glyph->bitmap.rows },
				face->glyph->bitmap.buffer,
				GL_RED
			};

			highest = std::max(highest, (u32)face->glyph->bitmap.rows);

			if (pos_btmp.x >= size_bitmap - face->glyph->bitmap.width)
			{
				pos_btmp.x = 0;
				pos_btmp.y += highest;
			}

			bitmap.insert(v2u32{ pos_btmp.x, pos_btmp.y }, glyph);


			u32 kerning{};
			if (FT_HAS_KERNING(face) && previous)
			{
				FT_Vector delta;
				FT_UInt glyph_index = FT_Get_Char_Index(face, c);
				FT_UInt prev_index = FT_Get_Char_Index(face, previous);
				FT_Get_Kerning(face, prev_index, glyph_index, FT_KERNING_DEFAULT, &delta);
				kerning = delta.x >> 6;
			}

			Character character{
				v2u32{pos_btmp.x , pos_btmp.y },
				glyph.getSize(),
				v2i32{face->glyph->bitmap_left, face->glyph->bitmap_top},
				face->glyph->advance.x,
				kerning
			};
			m_characters.insert(std::pair<u8, Character>{c, character});

			pos_btmp.x += (u32)face->glyph->bitmap.width + g_padding_pixels;

			previous = c;
		}

		// Release Resources
		FT_Done_Face(face);
		FT_Done_FreeType(ft);


		return bitmap;
	}
}
