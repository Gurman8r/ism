#ifndef _ISM_HASH_SET_HPP_
#define _ISM_HASH_SET_HPP_

#include <core/os/memory.hpp>
#include <core/templates/hash.hpp>

#include <unordered_set>

namespace ISM
{
	// hash_set
	template <class _Ty, class _Hash = Hash<_Ty>, class _KeyEq = EqualTo<_Ty>
	> ALIAS(HashSet) typename std::unordered_set<_Ty, _Hash, _KeyEq, PolymorphicAllocator<_Ty>>;

	// hash_multiset
	template <class _Ty, class _Hash = Hash<_Ty>, class _KeyEq = EqualTo<_Ty>
	> ALIAS(HashMultiset) typename std::unordered_multiset<_Ty, _Hash, _KeyEq, PolymorphicAllocator<_Ty>>;
}

#endif // !_ISM_HASH_SET_HPP_
