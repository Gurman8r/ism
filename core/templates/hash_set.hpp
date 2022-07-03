#ifndef _ISM_HASH_SET_HPP_
#define _ISM_HASH_SET_HPP_

#include <core/os/memory.hpp>
#include <core/templates/hash.hpp>

#include <unordered_set>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// hash set base
	template <class T, class H = Hasher<T>, class E = EqualTo<T>
	> ALIAS(_HashSetBase) std::unordered_set<T, H, E, PolymorphicAllocator<T>>;

	// hash set
	template <class T, class H = Hasher<T>, class E = EqualTo<T>
	> class HashSet : public _HashSetBase<T, H, E> {
	public:
		using base_type = typename _HashSetBase<T, H, E>;
		using base_type::base_type;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// multi hash set base
	template <class T, class H = Hasher<T>, class E = EqualTo<T>
	> ALIAS(_MultiHashSetBase) std::unordered_multiset<T, H, E, PolymorphicAllocator<T>>;

	// multi hash set
	template <class T, class H = Hasher<T>, class E = EqualTo<T>
	> class MultiHashSet : public _MultiHashSetBase<T, H, E> {
	public:
		using base_type = typename _MultiHashSetBase<T, H, E>;
		using base_type::base_type;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_HASH_SET_HPP_
