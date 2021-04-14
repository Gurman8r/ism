#include <core/io/image_loader.hpp>

#define STBI_FREE			memfree
#define STBI_MALLOC			memalloc
#define STBI_REALLOC		memrealloc
#define STBI_REALLOC_SIZED	memrealloc_sized

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace ism
{
	Error ImageLoader::load_image(Ref<Image> image, String const & path, bool flip_vertically, int32_t req_comp)
	{
		if (!image || path.empty()) { return Error_Unknown; }

		stbi_set_flip_vertically_on_load(flip_vertically);
		
		int32_t w, h, c;
		byte * const p = (byte *)stbi_load(path.c_str(), &w, &h, &c, req_comp);
		SCOPE_EXIT(&) { stbi_image_free(p); };
		if (!p) { return Error_Unknown; }
		
		image->m_width = w;
		image->m_height = h;
		image->m_channels = c;
		image->m_pixels = { p, p + w * h * c };
		return Error_None;
	}
}