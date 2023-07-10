#ifndef _ISM_IMAGE_LIBRARY_HPP_
#define _ISM_IMAGE_LIBRARY_HPP_

#include <core/io/image.hpp>
#include <core/io/resource_loader.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ImageFormatLoader : public ResourceFormatLoader
	{
		OBJECT_CLASS(ImageFormatLoader, ResourceFormatLoader);
	public:
		static Error_ load_image(Ref<Image> image, String const & path);
		virtual RES load(String const & path, Error_ * r_error = nullptr) override;
		virtual void get_recognized_extensions(Vector<String> * r_out) const override;

		static bool get_flip_vertically_on_load();
		static void set_flip_vertically_on_load(bool value);
		static void toggle_flip_vertically_on_load();
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_IMAGE_LIBRARY_HPP_
