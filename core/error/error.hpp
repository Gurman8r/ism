#ifndef _ISM_ERROR_HPP_
#define _ISM_ERROR_HPP_

// WIP

#include <core/typedefs.hpp>

namespace ism
{
	ALIAS(Error) int32_t;
	enum Error_ : Error
	{
		Error_Unknown = -1,
		Error_None = 0,
	};
}

#endif // !_ISM_ERROR_HPP_
