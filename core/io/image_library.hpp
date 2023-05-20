#ifndef _ISM_IMAGE_LIBRARY_HPP_
#define _ISM_IMAGE_LIBRARY_HPP_

#include <core/io/image.hpp>
#include <core/io/resource_loader.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ImageFormatLoader : public ResourceFormatLoader
	{
		DEFINE_CLASS(ImageFormatLoader, ResourceFormatLoader);
	public:
		static Error_ load_image(Ref<Image> image, String const & path);
		virtual RES load(String const & path, Error_ * r_error = nullptr) override;
		virtual void get_recognized_extensions(Vector<String> * out) const override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_IMAGE_LIBRARY_HPP_
