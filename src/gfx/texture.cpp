#include "texture.hpp"


namespace gfx
{

	// =====================
	// Construction/Destruction
	// =====================


	Texture::Texture(Type type) 
		: m_type{ type }
	{
		glBindTexture(m_type, m_id);

		glTexParameteri(m_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(m_type, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(m_type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(m_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	Texture::Texture(const filepath& tex_path, Type type)
		: Texture{ type }
	{
		load(tex_path);
	}

	Texture::Texture(u8* buffer, u32 size) 
		: Texture{ Type::tex2D }
	{
		load(buffer, size);
	}

	Texture::Texture(const Image& image) 
		: Texture{ Type::tex2D }
	{
		load(image);
	}


	// =====================
	// Actors
	// =====================

	void Texture::load(const filepath& tex_path)
	{
		if (!m_id.m_id) glGenTextures(1, &m_id.m_id);

		std::int32_t nrChannels{};

		stbi_set_flip_vertically_on_load(true);
		std::uint8_t* data{ stbi_load(tex_path.string().c_str(), &m_width, &m_height, &nrChannels, 0) };

		GLenum color_channel{};

		switch (nrChannels)
		{
		case 1:
			color_channel = GL_RED;
			break;

		case 3:
			color_channel = GL_RGB;
			break;

		case 4:
			color_channel = GL_RGBA;
			break;
		}

		glTexImage2D(m_type, 0, color_channel, m_width, m_height, 0, color_channel, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(m_type);

		stbi_image_free(data);
	}

	void Texture::load(u8* buffer, u32 size)
	{
		if (!m_id.m_id) glGenTextures(1, &m_id.m_id);

		std::int32_t nrChannels{};

		stbi_set_flip_vertically_on_load(true);
		std::uint8_t* data{ stbi_load_from_memory(buffer, size, &m_width, &m_height, &nrChannels, 0) };

		GLenum color_channel{};

		switch (nrChannels)
		{
		case 1:
			color_channel = GL_RED;
			break;

		case 3:
			color_channel = GL_RGB;
			break;

		case 4:
			color_channel = GL_RGBA;
			break;
		}

		glTexImage2D(m_type, 0, color_channel, m_width, m_height, 0, color_channel, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(m_type);

		stbi_image_free(data);
	}

	void Texture::load(const Image& image)
	{
		if (!m_id.m_id) glGenTextures(1, &m_id.m_id);

		m_width = static_cast<i32>(image.getSize().x);
		m_height = static_cast<i32>(image.getSize().y);

		glTexImage2D(m_type, 0, image.getFormat(), m_width, m_height, 0, image.getFormat(), GL_UNSIGNED_BYTE, image.getData().data());

		glGenerateMipmap(m_type);
	}

	void Texture::update(const Image& image) noexcept
	{
		glBindTexture(m_type, m_id);

		glTexImage2D(m_type, 0, image.getFormat(), image.getSize().x, image.getSize().y, 0, image.getFormat(), GL_UNSIGNED_BYTE, image.getData().data());
		m_width = image.getSize().x;
		m_height = image.getSize().y;

		glBindTexture(m_type, 0);
	}

	void Texture::unload() const noexcept
	{
		glDeleteTextures(1, &m_id.m_id);
	}

	void Texture::bind(uint32_t slot) const noexcept
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(m_type, m_id);
	}

	void Texture::unbind() const noexcept
	{
		glBindTexture(m_type, 0);
	}

}