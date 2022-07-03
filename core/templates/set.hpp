#ifndef _ISM_SET_HPP_
#define _ISM_SET_HPP_

#include <core/os/memory.hpp>

#include <set>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// set
	template <class T, class Pr = Less<T>
	> ALIAS(Set) std::set<T, Pr, PolymorphicAllocator<T>>;

	// multi set
	template <class T, class Pr = Less<T>
	> ALIAS(MultiSet) std::multiset<T, Pr, PolymorphicAllocator<T>>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_SET_HPP_
