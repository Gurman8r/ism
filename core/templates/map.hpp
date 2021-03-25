#ifndef _ISM_MAP_HPP_
#define _ISM_MAP_HPP_

#include <core/os/memory.hpp>

#include <map>

namespace ISM
{
#if 1
	// map
	template <class _Kt, class _Vt, class _Pr = Less<_Kt>
	> ALIAS(Map) typename std::map<_Kt, _Vt, _Pr, PolymorphicAllocator<std::pair<_Kt, _Vt>>>;
#else
	// map
	template <class _Kt, class _Vt, class _Pr = Less<_Kt>
	> class Map : public std::map<_Kt, _Vt, _Pr, PolymorphicAllocator<std::pair<_Kt const, _Vt>>>
	{
		using _Mybase = std::map<_Kt, _Vt, _Pr, PolymorphicAllocator<std::pair<_Kt const, _Vt>>>;
	public:
		using _Mybase::map;
		NODISCARD operator _Mybase & () & noexcept { return static_cast<_Mybase &>(*this); }
		NODISCARD operator _Mybase const & () const & noexcept { return static_cast<_Mybase const &>(*this); }
		NODISCARD operator _Mybase && () && noexcept { return static_cast<_Mybase &&>(std::move(*this)); }
	};
#endif

	template <class _Kt, class _Vt, class _Pr = Less<_Kt>, class T
	> NODISCARD bool contains(Map<_Kt, _Vt, _Pr> const & m, T && value)
	{
		return m.find(FWD(value)) != m.end();
	}

	template <class _Kt, class _Vt, class _Pr = Less<_Kt>, class T
	> NODISCARD _Vt * getptr(Map<_Kt, _Vt, _Pr> & m, T && value)
	{
		if (auto const it{ m.find(FWD(value)) }; it != m.end()) { return &it->second; }
		else { return nullptr; }
	}

	template <class _Kt, class _Vt, class _Pr = Less<_Kt>, class T
	> NODISCARD _Vt const * getptr(Map<_Kt, _Vt, _Pr> const & m, T && value)
	{
		if (auto const it{ m.find(FWD(value)) }; it != m.end()) { return &it->second; }
		else { return nullptr; }
	}
}

#endif // !_ISM_MAP_HPP_
