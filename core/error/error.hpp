#ifndef _ISM_ERROR_HPP_
#define _ISM_ERROR_HPP_

// WIP

#include <core/templates/type_traits.hpp>

namespace Ism
{
	enum Error_
	{
		Error_Failed = -1,
		Error_OK = 0,
		Error_EOF,
		// etc...

		Error_MAX
	};
}

#endif // !_ISM_ERROR_HPP_
