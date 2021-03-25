#ifndef _ISM_OPTIONAL_HPP_
#define _ISM_OPTIONAL_HPP_

#include <core/templates/type_traits.hpp>

#include <optional>

namespace ISM
{
	ALIAS(nullopt_t) std::nullopt_t;

	constexpr auto nullopt{ std::nullopt };

#if 0
	template <class _Ty
	> ALIAS(Optl) typename std::optional<_Ty>;
#else
	template <class _Ty
	> class Optl : public std::optional<_Ty>
	{
		using _Mybase = std::optional<_Ty>;
	public:
		using _Mybase::optional;
		NODISCARD operator _Mybase & () & noexcept { return static_cast<_Mybase &>(*this); }
		NODISCARD operator _Mybase const & () const & noexcept { return static_cast<_Mybase const &>(*this); }
		NODISCARD operator _Mybase && () && noexcept { return static_cast<_Mybase &&>(std::move(*this)); }
	};
#endif

	template <class T
	> NODISCARD Optl<T> make_optional(T && value) noexcept
	{
		return Optl<T>{ FWD(value) };
	}
}

#endif // !_ISM_OPTIONAL_HPP_
