#ifndef _ISM_HASH_SET_HPP_
#define _ISM_HASH_SET_HPP_

#include <core/os/memory.hpp>
#include <core/templates/hash.hpp>

#include <unordered_set>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// hash_set
	template <class _Ty, class _Hash = Hasher<_Ty>, class _KeyEq = EqualTo<_Ty>
	> ALIAS(HashSet) std::pmr::unordered_set<_Ty, _Hash, _KeyEq>;

	// hash_multiset
	template <class _Ty, class _Hash = Hasher<_Ty>, class _KeyEq = EqualTo<_Ty>
	> ALIAS(HashMultiset) std::pmr::unordered_multiset<_Ty, _Hash, _KeyEq>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_HASH_SET_HPP_
