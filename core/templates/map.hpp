#ifndef _ISM_MAP_HPP_
#define _ISM_MAP_HPP_

#include <core/os/memory.hpp>

#include <map>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// map
	template <class K, class V, class Pr = Less<K>
	> ALIAS(Map) std::map<K, V, Pr, PolymorphicAllocator<Pair<K const, V>>>;

	// multi map
	template <class K, class V, class Pr = Less<K>
	> ALIAS(MultiMap) std::multimap<K, V, Pr, PolymorphicAllocator<Pair<K const, V>>>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class K, class V, class Pr = Less<K>, class T
	> bool contains(Map<K, V, Pr> const & m, T && value)
	{
		return m.find(FWD(value)) != m.end();
	}

	template <class K, class V, class Pr = Less<K>, class T
	> V * getptr(Map<K, V, Pr> & m, T && value)
	{
		if (auto const it{ m.find(FWD(value)) }; it != m.end()) { return &it->second; }
		else { return nullptr; }
	}

	template <class K, class V, class Pr = Less<K>, class T
	> V const * getptr(Map<K, V, Pr> const & m, T && value)
	{
		if (auto const it{ m.find(FWD(value)) }; it != m.end()) { return &it->second; }
		else { return nullptr; }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_MAP_HPP_
