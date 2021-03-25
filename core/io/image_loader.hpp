#ifndef _ISM_IMAGE_LOADER_HPP_
#define _ISM_IMAGE_LOADER_HPP_

#include <core/io/image.hpp>

namespace ISM
{
	class ISM_API ImageLoader
	{
	public:
		static Err load_image(Ref<Image> image, String const & path, bool flip_vertically = true, int32_t req_comp = 4);
	};
}

#endif // !_ISM_IMAGE_LOADER_HPP_
