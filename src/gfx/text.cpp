#include "text.hpp"

#include <cassert>

#include "sys/assetsManager.hpp"
#include "vertices.hpp"


namespace gfx
{

	Text::Text(const Font* font, std::string_view str)
		: p_font{ font }, m_text{ str }, m_material{ &AssetsManager::get().shaders.at("shaders/text") }
	{
		m_material.set("tex", &p_font->get_tex());
		m_mesh.create_buffer<Vertex2D>(false);
		update();
	}

	void Text::draw(Renderer& renderer)
	{
		assert(p_font && "ERROR::TEXT::Cannot draw because no font is attached");

		m_material.set("TextColor"_id, m_color);
		renderer.push_command(&m_mesh, (m4f32)get_transform(), &m_material);
	}

	void Text::update()
	{
		if (!p_font)
			throw std::runtime_error("ERROR::TEXT::BUFFER_UPDATE: Cannot update buffer because no font is bound to the current text");

		std::vector<Vertex2D> data{};

		v2f32 tPos{};
		float height{};

		for (const auto& c : m_text)
		{
			Character ch{ p_font->get_character(c) };

			u8 cha = c;

			v2f32 pos{ tPos.x + (ch.bearing.x - ch.kerning) * m_scale_text, tPos.y - (static_cast<f32>(ch.size.y) - ch.bearing.y) * m_scale_text };
			v2f32 size{ ch.size.x * m_scale_text, ch.size.y * m_scale_text };

			data.insert(data.end(),
				{
					Vertex2D
					{ v2f32{pos.x, pos.y + size.y},						static_cast<v2f32>(v2u32 { ch.pos.x, ch.pos.y + ch.size.y })				},
					{ pos,												static_cast<v2f32>(ch.pos)													},
					{ v2f32{pos.x + size.x, pos.y},						static_cast<v2f32>(v2u32 { ch.pos.x + ch.size.x,	ch.pos.y })				},

					{ v2f32{pos.x,				pos.y + size.y},		static_cast<v2f32>(v2u32 { ch.pos.x,				ch.pos.y + ch.size.y })	},
					{ v2f32{pos.x + size.x,		pos.y},					static_cast<v2f32>(v2u32 { ch.pos.x + ch.size.x,	ch.pos.y })				},
					{ v2f32{pos.x + size.x,		pos.y + size.y},		static_cast<v2f32>(v2u32 { ch.pos.x + ch.size.x,	ch.pos.y + ch.size.y})	},
				});

			tPos.x += (ch.advance >> 6) * m_scale_text;
			height = std::max(height, ch.size.y * m_scale_text);
		}

		m_size_data = data.size();
		m_size = { tPos.x, height, 0 };

		m_mesh.update_buffer(data);
	}
}
