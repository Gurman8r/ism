#ifndef _ISM_IMAGE_LOADER_HPP_
#define _ISM_IMAGE_LOADER_HPP_

#include <core/io/image.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API ImageLoader final
	{
	public:
		/* load from file */
		static Error_ load_image(Image & image, Path const & path);
		static Error_ load_image(Ref<Image> image, Path const & path);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_IMAGE_LOADER_HPP_
