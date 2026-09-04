#pragma once // text.hpp
// MIT
// Allosker ------------------------------
// =========+
// This class manages a text entity that can be transformed
// ---------------------------------------

#include "sys/types.hpp"
#include "gfx/image.hpp"
#include "gfx/mesh.hpp"
#include "gfx/renderContext.hpp"

#include "gfx/transformable3D.hpp"

#include "font.hpp"


namespace gfx
{


	class Text
		: public Transformable3D
	{
	public:

		Text() noexcept = delete;

		Text(const Font* font, std::string_view str = {});

		Text(Text& other) noexcept;
		Text(Text&& other) noexcept;

		Text& operator=(Text& other) noexcept;
		Text& operator=(Text&& other) noexcept;

		~Text() noexcept;



		void set_str(const std::string& str)
		{
			m_text = str;
			update();
		}

		void set_color(const v4f32& color) noexcept { m_color = color; }
		/// <summary>
		/// Akin to Resolution
		/// </summary>
		/// <param name="scale"></param>
		void set_scale_text(f32 scale) noexcept { m_scale_text = scale; }
		
		void set_font(const Font& font)
		{
			p_font = &font;
			update();
		}



		void draw(const RenderContext& rc);

		void reload()
		{
			update();
		}


	private:


		void update();


	private:

		std::string m_text{};

		Mesh m_mesh{};

		const Font* p_font{ nullptr };

		v4f32 m_color		{ 0.5, 0.8, 0.2, 1. };
		f32 m_scale_text	{ 1. };


		size_t m_size_data{};

		GLuint m_vao{};
		GLuint m_vbo{};


	};


} // namespace render::gui