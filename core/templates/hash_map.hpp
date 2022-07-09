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
	> class HashMap : public _HashMapBase<K, V, H, E>
	{
	public:
		using base_type = _HashMapBase<K, V, H, E>;
		using base_type::base_type;
		using base_type::operator=;

		NODISCARD operator void * () const noexcept { return (void *)(intptr_t)!empty(); }

		NODISCARD bool contains(K const & key) const { return end() != find(key); }
		NODISCARD bool contains(K && key) const noexcept { return end() != find(FWD(key)); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// multi hashmap base
	template <class K, class V, class H = Hasher<K>, class E = EqualTo<K>
	> ALIAS(_MultiHashMapBase) std::unordered_multimap<K, V, H, E, PolymorphicAllocator<Pair<K const, V>>>;

	// multi hashmap
	template <class K, class V, class H = Hasher<K>, class E = EqualTo<K>
	> class MultiHashMap : public _MultiHashMapBase<K, V, H, E>
	{
	public:
		using base_type = _MultiHashMapBase<K, V, H, E>;
		using base_type::base_type;
		using base_type::operator=;

		NODISCARD operator void * () const noexcept { return (void *)(intptr_t)!empty(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
