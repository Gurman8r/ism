#ifndef _ISM_ERROR_HPP_
#define _ISM_ERROR_HPP_

// WIP

#include <core/templates/type_traits.hpp>

namespace ISM
{
	typedef enum Err_ : int32_t
	{
		Err_Unknown = -1,
		Err_None = 0,
	}
	Err;
}

#endif // !_ISM_ERROR_HPP_
