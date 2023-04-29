#ifndef _ISM_IMAGE_LIBRARY_HPP_
#define _ISM_IMAGE_LIBRARY_HPP_

#include <core/io/image.hpp>
#include <core/io/resource_loader.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API ImageLibrary final
	{
	public:
		static Error_ load_image(Image & image, String const & path);
		static Error_ load_image(Ref<Image> image, String const & path);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ImageFormatLoader : public ResourceFormatLoader
	{
		DEFINE_CLASS(ImageFormatLoader, ResourceFormatLoader);
	public:
		NODISCARD virtual RES load(String const & path, Error_ * error = nullptr) override;
		NODISCARD virtual void get_recognized_extensions(Vector<String> * out) const override;
		NODISCARD virtual TYPE get_resource_type(String const & path) const override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_IMAGE_LIBRARY_HPP_
