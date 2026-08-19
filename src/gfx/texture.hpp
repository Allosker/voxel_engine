#pragma once // texture.hpp
// MIT
// Allosker ------------------------------
// =========+
// Implement a small class that allocates a texture on the GPU (be it an atlas or anything). Based on a dynamic array of image data.
// ---------------------------------------

#include "sys/graphics.hpp"
#include "sys/types.hpp"

#include "image.hpp"


namespace gfx
{

	class Texture
	{
	public:

		enum Type
		{
			tex1D = GL_TEXTURE_1D,
			tex2D = GL_TEXTURE_2D,
			tex3D = GL_TEXTURE_3D
		};

	public:

	// = Construction/Destruction

		Texture(const filepath& tex_path, Type type = tex2D);

		Texture(const Image& image, Type type = tex2D);

		~Texture() noexcept;


	// = Actors

		void loadTexture(const filepath& tex_path);

		void update(const Image& image) noexcept;

		void deleteTexture() const noexcept;

		void bind() const noexcept;

		void unbind() const noexcept;


	// = Getters

		GLuint ID() const noexcept { return m_tex; }

		v2f32 getSize() const noexcept { return v2f32{ static_cast<float>(m_width), static_cast<float>(m_height) }; }


	private:

		Type m_type{};

		std::int32_t m_width{}, m_height{};

		GLuint m_tex{};

	};

} // render