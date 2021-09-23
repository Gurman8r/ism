#ifndef _ISM_FONT_HPP_
#define _ISM_FONT_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	API_CLASS(Font) : public Resource
	{
		OBJ_COMMON(Font, Resource);

	public:
		virtual ~Font();

		Font();

	private:

	};
}

#endif // !_ISM_FONT_HPP_
