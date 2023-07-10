#include <core/io/image_library.hpp>
#include <core/config/project_settings.hpp>

#define STBI_FREE memfree
#define STBI_MALLOC memalloc
#define STBI_REALLOC memrealloc
#define STBI_REALLOC_SIZED memrealloc_sized
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(ImageFormatLoader, t) {}

	Error_ ImageFormatLoader::load_image(Ref<Image> image, String const & path)
	{
		static ON_SCOPE_ENTER() { set_flip_vertically_on_load(true); };

		if (!image) { return Error_Failed; }
		if (path.empty()) { return Error_Failed; }
		u8 * data{ (u8 *)stbi_load(path.c_str(), &image->m_width, &image->m_height, &image->m_depth, 0) };
		ON_SCOPE_EXIT(&) { stbi_image_free(data); };
		if (!data) { return Error_Failed; }

		image->m_pixels = { data, data + image->m_width * image->m_height * image->m_depth };

		// TODO: properly deduce image format
		switch (image->m_depth) {
		case 1: { image->m_format = ImageFormat_R8; } break;
		case 2: { image->m_format = ImageFormat_RG8; } break;
		case 3: { image->m_format = ImageFormat_RGB8; } break;
		case 4: { image->m_format = ImageFormat_RGBA8; } break;
		}

		return Error_OK;
	}

	RES ImageFormatLoader::load(String const & path, Error_ * r_error)
	{
		Ref<Image> temp{}; temp.instance();
		if (Error_ const err{ load_image(temp, path) }) {
			if (r_error) { *r_error = err; }
			temp = nullptr;
		}
		else if (r_error) { *r_error = Error_OK; }
		return temp;
	}

	void ImageFormatLoader::get_recognized_extensions(Vector<String> * r_out) const
	{
		if (!r_out) { return; }
		r_out->push_back(".jpg");
		r_out->push_back(".jpeg");
		r_out->push_back(".png");
		r_out->push_back(".bmp");
	}

	bool ImageFormatLoader::get_flip_vertically_on_load()
	{
		return stbi__vertically_flip_on_load_global;
	}

	void ImageFormatLoader::set_flip_vertically_on_load(bool value)
	{
		stbi__vertically_flip_on_load_global = value;
	}

	void ImageFormatLoader::toggle_flip_vertically_on_load()
	{
		set_flip_vertically_on_load(!get_flip_vertically_on_load());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}
