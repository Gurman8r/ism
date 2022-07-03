#ifndef _ISM_HASH_MAP_HPP_
#define _ISM_HASH_MAP_HPP_

#include <core/os/memory.hpp>
#include <core/templates/hash.hpp>

#include <unordered_map>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// hash_map
	template <class K, class V, class H = Hasher<K>, class Eq = EqualTo<K>
	> ALIAS(HashMap) std::unordered_map<K, V, H, Eq, PolymorphicAllocator<Pair<K const, V>>>;

	// hash multi map
	template <class K, class V, class H = Hasher<K>, class Eq = EqualTo<K>
	> ALIAS(HashMultiMap) std::unordered_multimap<K, V, H, Eq, PolymorphicAllocator<Pair<K const, V>>>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class K, class V, class H = Hasher<K>, class Eq = EqualTo<K>, class T
	> bool has(HashMap<K, V, H, Eq> const & m, T && value)
	{
		return m.find(FWD(value)) != m.end();
	}

	template <class K, class V, class H = Hasher<K>, class Eq = EqualTo<K>, class T
	> V * getptr(HashMap<K, V, H, Eq> & m, T && value)
	{
		if (auto const it{ m.find(FWD(value)) }; it != m.end()) { return &it->second; }
		else { return nullptr; }
	}

	template <class K, class V, class H = Hasher<K>, class Eq = EqualTo<K>, class T
	> V const * getptr(HashMap<K, V, H, Eq> const & m, T && value)
	{
		if (auto const it{ m.find(FWD(value)) }; it != m.end()) { return &it->second; }
		else { return nullptr; }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_HASH_MAP_HPP_
