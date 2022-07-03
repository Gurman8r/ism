#ifndef _ISM_HASH_SET_HPP_
#define _ISM_HASH_SET_HPP_

#include <core/os/memory.hpp>
#include <core/templates/hash.hpp>

#include <unordered_set>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// hash set
	template <class T, class H = Hasher<T>, class Eq = EqualTo<T>
	> ALIAS(HashSet) std::unordered_set<T, H, Eq, PolymorphicAllocator<T>>;

	// hash multi set
	template <class T, class H = Hasher<T>, class Eq = EqualTo<T>
	> ALIAS(HashMultiSet) std::unordered_multiset<T, H, Eq, PolymorphicAllocator<T>>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_HASH_SET_HPP_
