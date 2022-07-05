#ifndef _ISM_MAP_HPP_
#define _ISM_MAP_HPP_

#include <core/os/memory.hpp>

#include <map>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// map base
	template <class K, class V, class Pr = Less<K>
	> ALIAS(_MapBase) std::map<K, V, Pr, PolymorphicAllocator<Pair<K const, V>>>;

	// map
	template <class K, class V, class Pr = Less<K>
	> class Map : public _MapBase<K, V, Pr>
	{
	public:
		using base_type = _MapBase<K, V, Pr>;
		using base_type::base_type;
		using base_type::operator=;

		NODISCARD bool contains(K const & key) const { return end() != find(key); }
		NODISCARD bool contains(K && key) const noexcept { return end() != find(FWD(key)); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// multi map base
	template <class K, class V, class Pr = Less<K>
	> ALIAS(_MultiMapBase) std::multimap<K, V, Pr, PolymorphicAllocator<Pair<K const, V>>>;

	// multi map
	template <class K, class V, class Pr = Less<K>
	> class MultiMap : public _MultiMapBase<K, V, Pr>
	{
	public:
		using base_type = _MultiMapBase<K, V, Pr>;
		using base_type::base_type;
		using base_type::operator=;
	};

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
