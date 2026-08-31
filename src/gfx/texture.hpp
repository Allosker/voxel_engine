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

		Texture(Type type = tex2D);

		Texture(const filepath& tex_path, Type type = tex2D);

		Texture(const Image& image);
		Texture(u8* buffer, u32 size);


		DELETE_COPY_INIT(Texture);

		Texture(Texture&& other) noexcept;
		Texture& operator=(Texture&& other) noexcept;

		~Texture() noexcept;


	// = Actors

		void load(const filepath& tex_path);
		void load_from_memory(u8* buffer, u32 size);

		void update(const Image& image) noexcept;

		void unload() const noexcept;

		void bind() const noexcept;

		void unbind() const noexcept;


	// = Getters

		GLuint id() const noexcept { return m_id; }

		v2f32 get_size() const noexcept { return v2f32{ static_cast<float>(m_width), static_cast<float>(m_height) }; }


	private:

		Type m_type{};

		std::int32_t m_width{}, m_height{};

		GLuint m_id{};

	};

} // render