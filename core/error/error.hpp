#ifndef _ISM_ERROR_HPP_
#define _ISM_ERROR_HPP_

// WIP

#include <core/templates/type_traits.hpp>

namespace ism
{
	typedef enum Error_ : int32_t
	{
		Error_Unknown = -1,
		Error_None = 0,
	}
	Error;
}

#endif // !_ISM_ERROR_HPP_
