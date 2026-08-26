#include "texture.hpp"


namespace gfx
{

	// =====================
	// Construction/Destruction
	// =====================

	Texture::Texture(const filepath& tex_path, Type type)
		: m_type{ type }
	{
		glGenTextures(1, &m_id);
		glBindTexture(m_type, m_id);

		glTexParameteri(m_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(m_type, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(m_type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(m_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		load(tex_path);
	}

	Texture::Texture(const Image& image, Type type)
		: m_type{ type }, m_width{ static_cast<i32>(image.getSize().x) }, m_height{ static_cast<i32>(image.getSize().y) }
	{
		glGenTextures(1, &m_id);
		glBindTexture(m_type, m_id);

		glTexParameteri(m_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(m_type, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(m_type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(m_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


		glTexImage2D(m_type, 0, image.getFormat(), m_width, m_height, 0, image.getFormat(), GL_UNSIGNED_BYTE, image.getData().data());

		glGenerateMipmap(m_type);
	}

	Texture::Texture(Texture&& other) noexcept
		: m_type{ other.m_type }, m_width{ other.m_width }, m_height{ other.m_height }, m_id{ other.m_id }
	{
		other.m_id = 0;
		other.m_height = 0;
		other.m_width = 0;
		other.m_type = {};
	}

	Texture& Texture::operator=(Texture && other) noexcept
	{
		if (this == &other)
			return *this;

		m_type = other.m_type;
		m_width = other.m_width;
		m_height = other.m_height;
		m_id = other.m_id;

		other.m_id = 0;
		other.m_height = 0;
		other.m_width = 0;
		other.m_type = {};

		return *this;
	}

	Texture::~Texture() noexcept
	{
		unload();
	}


	// =====================
	// Actors
	// =====================

	void Texture::load(const filepath& tex_path)
	{
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
		glDeleteTextures(1, &m_id);
	}

	void Texture::bind() const noexcept
	{
		glBindTexture(m_type, m_id);
	}

	void Texture::unbind() const noexcept
	{
		glBindTexture(m_type, 0);
	}

}