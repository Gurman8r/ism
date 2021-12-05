#include <core/io/image_loader.hpp>

#define STBI_FREE			memfree
#define STBI_MALLOC			memalloc
#define STBI_REALLOC		memrealloc
#define STBI_REALLOC_SIZED	memrealloc_sized

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

using namespace ism;

Error ImageLoader::load_image(Ref<Image> image, String const & path, bool flip_vertically, int32_t req_comp)
{
	if (!image || path.empty()) { return Error_Unknown; }
	
	stbi_set_flip_vertically_on_load(flip_vertically);
	
	int32_t width, height, channels;
	byte * data{ (byte *)stbi_load(path.c_str(), &width, &height, &channels, req_comp) };
	SCOPE_EXIT(data) { stbi_image_free(data); };
	if (!data) { return Error_Unknown; }

	image->m_width = width;
	image->m_height = height;
	image->m_channels = channels;
	image->m_pixels = { data, data + width * height * channels, PolymorphicAllocator<>{} };

	// TODO: properly deduce image format
	switch (channels)
	{
	case 1: { image->m_format = ImageFormat_R8; } break;
	case 2: { image->m_format = ImageFormat_RG8; } break;
	case 3: { image->m_format = ImageFormat_RGB8; } break;
	case 4: { image->m_format = ImageFormat_RGBA8; } break;
	}

	return Error_None;
}