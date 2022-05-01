#ifndef _ISM_FONT_HPP_
#define _ISM_FONT_HPP_

#include <core/io/resource.hpp>
#include <servers/text_server.hpp>

namespace ism
{
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
