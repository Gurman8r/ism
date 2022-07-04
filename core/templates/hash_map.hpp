#ifndef _ISM_HASH_MAP_HPP_
#define _ISM_HASH_MAP_HPP_

#include <core/os/memory.hpp>

#include <unordered_map>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// hashmap base
	template <class K, class V, class H = Hasher<K>, class E = EqualTo<K>
	> ALIAS(_HashMapBase) std::unordered_map<K, V, H, E, PolymorphicAllocator<Pair<K const, V>>>;

	// hashmap
	template <class K, class V, class H = Hasher<K>, class E = EqualTo<K>
	> class HashMap : public _HashMapBase<K, V, H, E> {
	public:
		using base_type = _HashMapBase<K, V, H, E>;
		using base_type::base_type;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// multi hashmap base
	template <class K, class V, class H = Hasher<K>, class E = EqualTo<K>
	> ALIAS(_MultiHashMapBase) std::unordered_multimap<K, V, H, E, PolymorphicAllocator<Pair<K const, V>>>;

	// multi hashmap
	template <class K, class V, class H = Hasher<K>, class E = EqualTo<K>
	> class MultiHashMap : public _MultiHashMapBase<K, V, H, E> {
	public:
		using base_type = _MultiHashMapBase<K, V, H, E>;
		using base_type::base_type;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class K, class V, class H = Hasher<K>, class E = EqualTo<K>, class T
	> bool has(HashMap<K, V, H, E> const & m, T && value)
	{
		return m.find(FWD(value)) != m.end();
	}

	template <class K, class V, class H = Hasher<K>, class E = EqualTo<K>, class T
	> V * getptr(HashMap<K, V, H, E> & m, T && value)
	{
		if (auto const it{ m.find(FWD(value)) }; it != m.end()) { return &it->second; }
		else { return nullptr; }
	}

	template <class K, class V, class H = Hasher<K>, class E = EqualTo<K>, class T
	> V const * getptr(HashMap<K, V, H, E> const & m, T && value)
	{
		if (auto const it{ m.find(FWD(value)) }; it != m.end()) { return &it->second; }
		else { return nullptr; }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_HASH_MAP_HPP_
