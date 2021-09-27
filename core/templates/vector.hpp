#ifndef _ISM_VECTOR_HPP_
#define _ISM_VECTOR_HPP_

#include <core/os/memory.hpp>

#include <forward_list>
#include <vector>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// vector
	template <class _Ty
	> ALIAS(Vector) std::vector<_Ty, PolymorphicAllocator<_Ty>>;

	// forward_list
	template <class _Ty
	> ALIAS(ForwardList) std::forward_list<_Ty, PolymorphicAllocator<_Ty>>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class _Ty, class T
	> NODISCARD bool has(Vector<_Ty> const & l, T && value)
	{
		return l.end() != std::find(l.begin(), l.end(), FWD(value));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_VECTOR_HPP_
