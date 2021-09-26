#ifndef _ISM_OPTIONAL_HPP_
#define _ISM_OPTIONAL_HPP_

#include <core/templates/type_traits.hpp>

#include <optional>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ALIAS(nullopt_t) std::nullopt_t;

	constexpr nullopt_t nullopt{ std::nullopt };

	template <class T> ALIAS(Optional) std::optional<T>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> NODISCARD constexpr auto make_optional(T && value)
	{
		return Optional<std::decay_t<T>>{ FWD(value) };
	}

	template <class T, class ... Args
	> NODISCARD constexpr auto make_optional(Args && ... args)
	{
		return Optional<T>{ std::in_place, FWD(args)... };
	}

	template <class T, class Elem, class ... Args
	> NODISCARD constexpr auto make_optional(std::initializer_list<Elem> init, Args&&... args)
	{
		return Optional<T>{ std::in_place, init, FWD(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_OPTIONAL_HPP_
