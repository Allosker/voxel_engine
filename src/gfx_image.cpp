#include "gfx_image.hpp"


namespace gfx
{

	Image::Image(const filepath& path_to_image, bool flip_on_load)
	{
		load_image(path_to_image, flip_on_load);
	}

	Image::Image(v2u32 size, u8* ptr, GLenum format)
		: m_size{ size }, m_format{ format }
	{
		m_data.insert(m_data.end(), ptr, ptr + size.x * size.y);
	}

	Image::Image(v2u32 allocate_size, GLenum format)
		:m_size{ allocate_size }, m_format{ format }
	{
		m_data.resize(allocate_size.x * allocate_size.y * getChannel());
	}


	u32 Image::getChannel() const noexcept
	{
		switch (m_format)
		{
		case GL_RED:
			return 1;
			break;

		case GL_RGB:
			return 3;
			break;

		case GL_RGBA:
			return 4;
			break;
		}

		return 0;
	}

	std::vector<u8> Image::crop(v2u32 subset_ori, v2u32 subset_size)
	{
		v2i32 subset_place{ subset_ori + subset_size };
		if ((subset_size.x > m_size.x || subset_size.y > m_size.y) || (subset_place.x > m_size.x || subset_place.y > m_size.y))
			throw std::runtime_error("ERROR::Subset out of bounds");

		std::vector<u8> new_data{};
		new_data.reserve(subset_size.x * subset_size.y * getChannel());

		for (u32 y{ subset_ori.y }; y < subset_place.y; y++)
		{
			for (u32 x{ subset_ori.x }; x < subset_place.x; x++)
			{
				for (u32 p{}; p < getChannel(); p++)
					new_data.emplace_back(m_data[(x + y * m_size.x) * getChannel() + p]);
			}
		}

		return new_data;
	}

	void Image::insert(v2u32 pos, const Image& other)
	{
		v2u32 subset_place{ pos + other.getSize() };
		if ((other.getSize().x > m_size.x || other.getSize().y > m_size.y) || (subset_place.x > m_size.x || subset_place.y > m_size.y))
			throw std::runtime_error("ERROR::Subset out of bounds");

		for (u32 y{ pos.y }; y < subset_place.y; y++)
		{
			for (u32 x{ pos.x }; x < subset_place.x; x++)
			{
				for (u32 p{}; p < getChannel(); p++)
					m_data[(x + y * m_size.x) * getChannel() + p] = other.getData()[((x - pos.x) + (y - pos.y) * other.getSize().x) * getChannel() + p];
			}
		}

	}


	/*private*/ void Image::load_image(const filepath& path, bool flip_on_load)
	{
		stbi_set_flip_vertically_on_load(flip_on_load);

		int channels{};
		v2i32 size_truncated{};
		std::uint8_t* data{ stbi_load(path.string().c_str(), &size_truncated.x, &size_truncated.y, &channels, 0) };
		m_size = size_truncated;
		u32 size{ m_size.x * m_size.y * channels };

		m_data.assign(data, data + size);

		switch (channels)
		{
		case 1:
			m_format = GL_RED;
			break;

		case 3:
			m_format = GL_RGB;
			break;

		case 4:
			m_format = GL_RGBA;
			break;
		}


		stbi_image_free(data);
	}
}
