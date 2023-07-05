#ifndef _ISM_ERROR_HPP_
#define _ISM_ERROR_HPP_

// WIP

#include <core/templates/type_traits.hpp>

namespace Ism
{
	enum Error_
	{
		Error_Unknown = -1	, // unknown
		Error_OK = 0		, // OK
		Error_Failed		, // failed
		Error_Bug			, // bug
		Error_EOF			, // end of file
		Error_AlreadyExists	, // already exists
		Error_Unavailable	, // unavailable

		// etc...

		Error_MAX
	};
}

#endif // !_ISM_ERROR_HPP_
