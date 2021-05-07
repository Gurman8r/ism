#ifndef _ISM_OPTIONAL_HPP_
#define _ISM_OPTIONAL_HPP_

#include <core/templates/type_traits.hpp>

#include <optional>

namespace ism
{
	ALIAS(nullopt_t) std::nullopt_t;

	constexpr auto nullopt{ std::nullopt };

	template <class T> ALIAS(Optional) typename std::optional<T>;

	template <class T
	> NODISCARD Optional<T> make_optional(T && value) noexcept
	{
		return Optional<T>{ FWD(value) };
	}

	template <class T, class ... Args
	> NODISCARD Optional<T> make_optional(Args && ... args) noexcept
	{
		return Optional<T>{ FWD(args)... };
	}
}

#endif // !_ISM_OPTIONAL_HPP_
