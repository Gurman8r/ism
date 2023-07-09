#ifndef _ISM_FONT_LIBRARY_HPP_
#define _ISM_FONT_LIBRARY_HPP_

#include <scene/resources/font.hpp>
#include <core/io/resource_loader.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class FontFormatLoader : public ResourceFormatLoader {
		OBJECT_CLASS(FontFormatLoader, ResourceFormatLoader);
	public:
		static Error_ load_font(Ref<Font> font, String const & path);
		virtual RES load(String const & path, Error_ * r_error = nullptr) override;
		virtual void get_recognized_extensions(Vector<String> * out) const override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_FONT_LIBRARY_HPP_
