#ifndef _ISM_TUPLE_HPP_
#define _ISM_TUPLE_HPP_

#include <core/templates/utility.hpp>

#include <tuple>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Type0, class ... Types
	> ALIAS(Tuple) std::tuple<Type0, Types...>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_TUPLE_HPP_
