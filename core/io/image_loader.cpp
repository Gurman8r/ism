#include <core/io/image_loader.hpp>

using namespace ism;

#define STBI_FREE memfree
#define STBI_MALLOC memalloc
#define STBI_REALLOC memrealloc
#define STBI_REALLOC_SIZED memrealloc_sized
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Error_ ImageLoader::load_image(Image & image, Path const & path)
{
	if (!path) { return Error_Unknown; }

	static SCOPE_ENTER() { stbi_set_flip_vertically_on_load(true); };
	byte * data{ (byte *)stbi_load(path.c_str(), &image.m_width, &image.m_height, &image.m_depth, 0) };
	SCOPE_EXIT(data) { stbi_image_free(data); };
	if (!data) { return Error_Unknown; }

	image.m_pixels = { data, data + image.m_width * image.m_height * image.m_depth };

	// TODO: properly deduce image format
	switch (image.m_depth) {
	case 1: { image.m_format = Image::Format_R8; } break;
	case 2: { image.m_format = Image::Format_RG8; } break;
	case 3: { image.m_format = Image::Format_RGB8; } break;
	case 4: { image.m_format = Image::Format_RGBA8; } break;
	}

	return Error_None;
}

Error_ ImageLoader::load_image(Ref<Image> image, Path const & path)
{
	if (!image) { return Error_Unknown; }
	return load_image(**image, path);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
