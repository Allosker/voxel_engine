#include "text.hpp"
#include <cassert>
#include <utility>


namespace gfx
{

	Text::Text(const Font* font, std::string_view str)
		: p_font{ font }, m_text{ str }
	{
		// create GPU data

		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(Vertex2D), std::bit_cast<void*>(offsetof(Vertex2D, pos)));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(Vertex2D), std::bit_cast<void*>(offsetof(Vertex2D, uvs)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

		update();
	}

	Text::Text(Text&& other) noexcept
		: Transformable3D{ static_cast<Transformable3D>(other) }
		, p_font{ other.p_font }
		, m_text{ std::move(other.m_text) }
		, m_color{ other.m_color }
		, m_scale_text{ other.m_scale_text }
		, m_size_data{ other.m_size_data }
		, m_vao{ other.m_vao }
		, m_vbo{ other.m_vbo }
	{
		other.m_vao = 0;
		other.m_vbo = 0;
	}

	Text& Text::operator=(Text&& other) noexcept
	{
		if (this == &other) return *this;

		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);

		Transformable3D::operator=(std::move(other));
		p_font = other.p_font;
		m_text = std::move(other.m_text);
		m_color = other.m_color;
		m_scale_text = other.m_scale_text;
		m_size_data = other.m_size_data;
		m_vao = other.m_vao;
		m_vbo = other.m_vbo;

		other.m_vao = 0;
		other.m_vbo = 0;

		return *this;
	}

	Text::~Text() noexcept
	{
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
	}


	void Text::draw(const RenderContext& rc)
	{
		assert(p_font && "ERROR::TEXT::Cannot draw because no font is attached");

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		p_font->bind();

		rc.sha->set_value("TextColor", m_color);
		rc.sha->set_value("model", get_transform());


		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLES, 0, m_size_data);
		glBindVertexArray(0);
	}

	void Text::update()
	{
		if (!p_font)
			throw std::runtime_error("ERROR::TEXT::BUFFER_UPDATE: Cannot update buffer because no font is bound to the current text");

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		std::vector<Vertex2D> data{};

		v2f32 tPos{};
		float height{};

		for (const auto& c : m_text)
		{
			Character ch{ p_font->getCharacter(c) };

			u8 cha = c;

			v2f32 pos{ tPos.x + (ch.bearing.x - ch.kerning) * m_scale_text, tPos.y - (static_cast<f32>(ch.size.y) - ch.bearing.y) * m_scale_text };
			v2f32 size{ ch.size.x * m_scale_text, ch.size.y * m_scale_text };

			data.insert(data.end(),
				{
					Vertex2D
					{ v2f32{pos.x, pos.y + size.y},						static_cast<v2f32>(ch.pos)												},
					{ pos,												static_cast<v2f32>(v2u32 {ch.pos.x, ch.pos.y + ch.size.y})				},
					{ v2f32{pos.x + size.x, pos.y},						static_cast<v2f32>(v2u32 {ch.pos.x + ch.size.x,	ch.pos.y + ch.size.y})	},

					{ v2f32{pos.x,				pos.y + size.y},		static_cast<v2f32>(v2u32 {ch.pos.x,				ch.pos.y})				},
					{ v2f32{pos.x + size.x,		pos.y},					static_cast<v2f32>(v2u32 {ch.pos.x + ch.size.x,	ch.pos.y + ch.size.y})	},
					{ v2f32{pos.x + size.x,		pos.y + size.y},		static_cast<v2f32>(v2u32 {ch.pos.x + ch.size.x,	ch.pos.y})				},
				});
			
			tPos.x += (ch.advance >> 6) * m_scale_text;
			height = std::max(height, ch.size.y * m_scale_text);
		}

		m_size_data = data.size();

		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2D) * data.size(), data.data(), GL_STREAM_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}
