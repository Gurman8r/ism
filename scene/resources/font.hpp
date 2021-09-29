#ifndef _ISM_FONT_HPP_
#define _ISM_FONT_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	class Texture2D;

	struct NODISCARD Glyph
	{
		Ref<Texture2D> graphic{};
		Rect bounds{};
		uint32_t advance{};
	};

	class ISM_API Font : public Resource
	{
		OBJECT_COMMON(Font, Resource);

	public:
		virtual ~Font();

		Font();

	private:

	};
}

#endif // !_ISM_FONT_HPP_
