#ifndef _ISM_SET_HPP_
#define _ISM_SET_HPP_

#include <core/os/memory.hpp>

#include <set>

namespace ISM
{
#if 1
	// set
	template <class _Ty, class _Pr = Less<_Ty>
	> ALIAS(Set) typename std::set<_Ty, _Pr, PolymorphicAllocator<_Ty>>;
#else
	// set
	template <class _Ty, class _Pr = Less<_Ty>
	> class Set : public std::set<_Ty, _Pr, PolymorphicAllocator<_Ty>>
	{
		using _Mybase = std::set<_Ty, _Pr, PolymorphicAllocator<_Ty>>;
	public:
		using _Mybase::set;
		NODISCARD operator _Mybase & () & noexcept { return static_cast<_Mybase &>(*this); }
		NODISCARD operator _Mybase const & () const & noexcept { return static_cast<_Mybase const &>(*this); }
		NODISCARD operator _Mybase && () && noexcept { return static_cast<_Mybase &&>(std::move(*this)); }
	};
#endif
}

#endif // !_ISM_SET_HPP_
