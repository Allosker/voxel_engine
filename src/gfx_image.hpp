#pragma once // image.hpp
// MIT
// Allosker ------------------------------
// =========+
// This class manages an image on RAM to allow for trasnformation thereof. This class is mainly a helper for managing image data before sending it back to the GPU as a Texture.
// ---------------------------------------

#include <vector>
#include <filesystem>


#include <stb/stb_image.h>


#include "sys_graphics.hpp"
#include "sys_types.hpp"


namespace gfx
{

	class Image
	{
	public:


		Image(const filepath& path_to_image, bool flip_on_load = true);

		Image(v2u32 size, u8* ptr, GLenum format);

		Image(v2u32 allocate_size, GLenum format);


		Image(Image&&) = default;
		Image& operator=(Image&&) = default;

		Image() = delete;
		Image(Image&) = delete;
		Image& operator=(Image&) = delete;


		// = Getters 

		std::vector<u8>& getData() noexcept { return m_data; }
		const std::vector<u8>& getData() const noexcept { return m_data; }

		v2u32 getSize() const noexcept { return m_size; }

		u32 getChannel() const noexcept;

		GLenum getFormat() const noexcept { return m_format; }

		i32 getBytesPerPixel() const noexcept { return getChannel(); }

		u8* data() noexcept { return m_data.data(); }
		const u8* data() const noexcept { return m_data.data(); }


		// = Setters

		void resize(v2u32 new_size) noexcept 
		{
			m_data.resize(new_size.x * new_size.y); 
			m_size = new_size;
		}

		std::vector<u8> crop(v2u32 subset_ori, v2u32 subset_size);

		void insert(v2u32 pos, const Image& other);





	private:

		void load_image(const filepath& path, bool flip_on_load);


	private:

		std::vector<u8> m_data{};
		v2u32 m_size{};

		GLenum m_format{};

	};

}