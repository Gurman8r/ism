#ifndef _ISM_HASH_MAP_HPP_
#define _ISM_HASH_MAP_HPP_

#include <core/os/memory.hpp>
#include <core/templates/hash.hpp>

#include <unordered_map>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// hash_map
	template <class _Kt, class _Vt, class _Hash = Hasher<_Kt>, class _KeyEq = EqualTo<_Kt>
	> ALIAS(HashMap) std::unordered_map<_Kt, _Vt, _Hash, _KeyEq, PolymorphicAllocator<std::pair<_Kt const, _Vt>>>;

	// hash_multimap
	template <class _Kt, class _Vt, class _Hash = Hasher<_Kt>, class _KeyEq = EqualTo<_Kt>
	> ALIAS(HashMultimap) std::unordered_multimap<_Kt, _Vt, _Hash, _KeyEq, PolymorphicAllocator<std::pair<_Kt const, _Vt>>>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class _Kt, class _Vt, class _Hash = Hasher<_Kt>, class _KeyEq = EqualTo<_Kt>, class T
	> NODISCARD bool has(HashMap<_Kt, _Vt, _Hash, _KeyEq> const & m, T && value)
	{
		return m.find(FWD(value)) != m.end();
	}

	template <class _Kt, class _Vt, class _Hash = Hasher<_Kt>, class _KeyEq = EqualTo<_Kt>, class T
	> NODISCARD _Vt * getptr(HashMap<_Kt, _Vt, _Hash, _KeyEq> & m, T && value)
	{
		if (auto const it{ m.find(FWD(value)) }; it != m.end()) { return &it->second; }
		else { return nullptr; }
	}

	template <class _Kt, class _Vt, class _Hash = Hasher<_Kt>, class _KeyEq = EqualTo<_Kt>, class T
	> NODISCARD _Vt const * getptr(HashMap<_Kt, _Vt, _Hash, _KeyEq> const & m, T && value)
	{
		if (auto const it{ m.find(FWD(value)) }; it != m.end()) { return &it->second; }
		else { return nullptr; }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_HASH_MAP_HPP_
