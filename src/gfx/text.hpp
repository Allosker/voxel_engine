#pragma once // text.hpp
// MIT
// Allosker ------------------------------
// =========+
// This class manages a text entity that can be transformed
// ---------------------------------------

#include "gfx/image.hpp"
#include "gfx/mesh.hpp"
#include "gfx/renderContext.hpp"
#include "sys/types.hpp"

#include "gfx/transformable3D.hpp"

#include "font.hpp"
#include "shader.hpp"


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

		void set_font(const Font* font)
		{
			p_font = font;
			update();
		}


		const std::string& get_str() const noexcept { return m_text; }

		v3f64 get_size() const noexcept { return m_size * get_scale(); }


		void draw(const Shader& sha);

		void reload()
		{
			update();
		}


	private:


		void update();


	private:

		std::string m_text{};

		const Font* p_font{ nullptr };

		v3f64 m_size{};

		v4f32 m_color{ 0.18, 0.18, 0.18, 1. };
		f32 m_scale_text{ 1. };

		size_t m_size_data{};

		GLuint m_vao{};
		GLuint m_vbo{};


	};


} // namespace render::gui