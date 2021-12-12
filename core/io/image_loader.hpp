#ifndef _ISM_IMAGE_LOADER_HPP_
#define _ISM_IMAGE_LOADER_HPP_

#include <core/io/image.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API ImageLoader
	{
	public:
		static Error_ load_image(Ref<Image> image, Path const & path);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_IMAGE_LOADER_HPP_
