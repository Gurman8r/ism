#ifndef _ISM_FONT_LIBRARY_HPP_
#define _ISM_FONT_LIBRARY_HPP_

#include <scene/resources/font.hpp>

namespace ism
{
	class ISM_API FontLibrary final
	{
	public:
		static Error_ load_font(Font & font, Path const & path);
		static Error_ load_font(Ref<Font> font, Path const & path);
	};
}

#endif // !_ISM_FONT_LIBRARY_HPP_
