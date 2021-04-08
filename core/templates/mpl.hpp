#ifndef _ISM_MPL_HPP_
#define _ISM_MPL_HPP_

// Sources:
// https://www.youtube.com/watch?v=NTWSeQtHZ9M
// https://github.com/SuperV1234/cppcon2015
// https://github.com/SuperV1234/cppcon2015/tree/master/Other/ecs/Utils/MPL
// https://github.com/SuperV1234/cppcon2015/blob/master/Other/ecs/Utils/MetaFor.hpp
// https://github.com/SuperV1234/cppcon2015/blob/master/Other/ecs/Utils/MPL/TypeListOps.hpp
// https://stackoverflow.com/questions/18063451/get-index-of-a-tuple-elements-type
// https://stackoverflow.com/questions/25958259/how-do-i-find-out-if-a-tuple-contains-a-type
// https://stackoverflow.com/questions/37029886/how-to-construct-a-tuple-from-an-array
// https://stackoverflow.com/questions/36580855/construct-tuple-by-passing-the-same-argument-to-each-element-with-explicit-const
// https://stackoverflow.com/questions/22560100/how-to-initialize-all-tuple-elements-by-the-same-arguments

#include <core/templates/utility.hpp>

#define MPL_TAG(type_tag) decltype(type_tag)::type

// MPL STRUCTURES
namespace ISM::MPL
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// type_list
	template <class ... Ts
	> struct type_list { static constexpr size_t size{ sizeof...(Ts) }; };

	// type_tag
	template <class T
	> struct type_tag { using type = typename T; };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// LOOPS
namespace ISM::MPL
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Tup, class Fn, size_t ... Is
	> constexpr void impl_tuple_expand(Tup && tp, Fn && fn, std::index_sequence<Is...>) noexcept
	{
		fn(std::get<Is>(FWD(tp))...);
	}

	template <class Tup, class Fn
	> constexpr void impl_tuple_expand(Tup && tp, Fn && fn) noexcept
	{
		MPL::impl_tuple_expand(
			FWD(tp),
			FWD(fn),
			std::make_index_sequence<std::tuple_size_v<std::decay_t<Tup>>>());
	}

	// "unpacks" the contents of a tuple inside a function call
	template <class Tup, class Fn
	> constexpr void tuple_expand(Tup && tp, Fn && fn) noexcept
	{
		MPL::impl_tuple_expand(FWD(tp), FWD(fn));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Fn, class ... Args
	> constexpr void impl_for_args(Fn && fn, Args && ... args) noexcept
	{
		if constexpr (0 < sizeof...(args)) // sanity check
		{
			(void)std::initializer_list<int>{ (fn(FWD(args)), 0)... };
		}
	}

	// invoke function on every argument
	template <class Fn, class ... Args
	> constexpr void for_args(Fn && fn, Args && ... args) noexcept
	{
		MPL::impl_for_args(FWD(fn), FWD(args)...);
	}

	// invoke function on every argument with index
	template <class Fn, class ... Args
	> constexpr void for_args_i(Fn && fn, Args && ... args) noexcept
	{
		size_t i{};
		MPL::for_args([&](auto && e) noexcept
		{
			fn(i++, FWD(e));
		}
		, FWD(args)...);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Tup, class Fn
	> constexpr void impl_for_tuple(Tup && tp, Fn && fn) noexcept
	{
		MPL::impl_tuple_expand(FWD(tp), [&fn](auto && ... rest) noexcept
		{
			MPL::for_args(fn, FWD(rest)...);
		});
	}

	// invoke function on every tuple element
	template <class Tup, class Fn
	> constexpr void for_tuple(Tup && tp, Fn && fn) noexcept
	{
		MPL::impl_for_tuple(FWD(tp), FWD(fn));
	}

	// invoke function on every tuple element with index
	template <class Tup, class Fn
	> constexpr void for_tuple_i(Tup && tp, Fn && fn) noexcept
	{
		size_t i{};
		MPL::for_tuple(FWD(tp), [&](auto && e) noexcept
		{
			fn(i++, FWD(e));
		});
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// CONCAT - combine types
namespace ISM::MPL
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ...
	> struct impl_concat
	{
		using type = typename type_list<>;
	};

	template <class ... Ts
	> ALIAS(concat) typename impl_concat<Ts...>::type;

	template <class ... Ts
	> struct impl_concat<type_list<Ts...>>
	{
		using type = typename type_list<Ts...>;
	};

	template <class ... Ts0, class ... Ts1, class ... Rest
	> struct impl_concat<type_list<Ts0...>, type_list<Ts1...>, Rest...>
	{
		using type = typename concat<type_list<Ts0..., Ts1...>, Rest...>;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// REMAP - modify inner types
namespace ISM::MPL
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <template <class> class Pr, class
	> struct impl_remap
	{
		using type = typename type_list<>;
	};

	template <template <class> class Pr, class Ls
	> ALIAS(remap) typename impl_remap<Pr, Ls>::type;

	template <template <class> class Pr, class T, class ... Ts
	> struct impl_remap<Pr, type_list<T, Ts...>>
	{
		using type = typename concat<type_list<Pr<T>>, remap<Pr, type_list<Ts...>>>;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// RENAME - modify outer type
namespace ISM::MPL
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <template <class...> class To, class T
	> struct impl_rename;

	template <
		template <class ...> class To,
		template <class...> class From,
		class ... Ts
	> struct impl_rename<To, From<Ts...>>
	{
		using type = typename To<Ts...>;
	};

	template<
		template <class...> class To, class T
	> ALIAS(rename) typename impl_rename<To, T>::type;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// CONTAINS - check type_list contains type
namespace ISM::MPL
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class Ls
	> struct contains;

	template <class T
	> struct contains<T, type_list<>>
		: std::false_type {};

	template <class T, class U, class ... Ts
	> struct contains<T, type_list<U, Ts...>>
		: contains<T, type_list<Ts...>> {};

	template <class T, class ... Ts
	> struct contains<T, type_list<T, Ts...>>
		: std::true_type {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// INDEX OF - get index of type in a type_list
namespace ISM::MPL
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class Ls
	> struct index_of;

	template <class T, class ... Ts
	> struct index_of<T, type_list<T, Ts...>>
		: std::integral_constant<size_t, 0> {};

	template <class T, class U, class ... Ts
	> struct index_of<T, type_list<U, Ts...>>
		: std::integral_constant<size_t, 1 + index_of<T, type_list<Ts...>>::value> {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// GENERAL
namespace ISM::MPL
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	template <class Ls
	> ALIAS(tuple) typename rename<std::tuple, Ls>;
	
	template <class Ls
	> ALIAS(tag_tuple) typename tuple<remap<type_tag, Ls>>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Ls, class Fn
	> constexpr void for_type_list(Fn && fn) noexcept
	{
		MPL::for_tuple(MPL::tag_tuple<Ls>{}, fn);
	}

	template <class T0, class ... Ts, class Fn
	> constexpr void for_types(Fn && fn) noexcept
	{
		MPL::for_type_list<MPL::type_list<T0, Ts...>>(FWD(fn));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Ls, class Fn
	> constexpr void for_type_list_i(Fn && fn) noexcept
	{
		size_t i{};
		MPL::for_type_list<Ls>([&](auto tag) noexcept
		{
			fn(i++, tag);
		});
	}

	template <class ... Ts, class Fn
	> constexpr void for_types_i(Fn && fn) noexcept
	{
		size_t i{};
		MPL::for_types<Ts...>([&](auto tag) noexcept
		{
			fn(i++, tag);
		});
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Ls
	> constexpr size_t size() noexcept { return Ls::size; }

	template <class Ls, class T> ALIAS(push_back) typename concat<Ls, type_list<T>>;

	template <class Ls, class T> ALIAS(push_front) typename concat<type_list<T>, Ls>;

	template <size_t I, class Ls> ALIAS(nth) typename std::tuple_element_t<I, tuple<Ls>>;

	template <class Ls> ALIAS(head) typename nth<0, Ls>;

	template <class Ls> ALIAS(tail) typename nth<size<Ls>() - 1, Ls>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// REPEAT - size N type_list of T
namespace ISM::MPL
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <size_t N, class T
	> struct impl_repeat
	{
		using type = typename push_back<typename impl_repeat<N - 1, T>::type, T>;
	};

	template <class T
	> struct impl_repeat<0, T>
	{
		using type = typename type_list<>;
	};

	template <size_t N, class T
	> ALIAS(repeat) typename impl_repeat<N, T>::type;

	template <class T, size_t N
	> ALIAS(array) typename tuple<repeat<N, T>>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// FILTER - remove types from type_list
namespace ISM::MPL
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <template <class> class Pr, class
	> struct impl_filter
	{
		using type = typename type_list<>;
	};

	template <template <class> class Pr, class Ls
	> ALIAS(filter) typename impl_filter<Pr, Ls>::type;

	template <template <class> class Pr, class T, class ... Ts
	> struct impl_filter<Pr, type_list<T, Ts...>>
	{
		using next = typename filter<Pr, type_list<Ts...>>;

		using type = typename std::conditional_t<
			(Pr<T>{}),
			concat<type_list<T>, next>,
			next
		>;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// ALL - used to run a check against all types in a type_list
namespace ISM::MPL
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <template <class> class, class...
	> struct all
		: std::true_type {};

	template <template <class> class Pr, class T, class ... Ts
	> struct all<Pr, T, Ts...>
		: std::bool_constant<(Pr<T>{} && all<Pr, Ts...>{})> {};

	template <template <class> class TMF
	> struct bound_all
	{
		template <class ... Ts
		> using type = typename all<TMF, Ts...>;
	};

	template <template <class> class TMF, class TL
	> ALIAS(all_types) typename rename<bound_all<TMF>::template type, TL>;

	// example
	static_assert(all<std::is_const, int const>{});
	static_assert(all_types<std::is_const, type_list<int const, float const>>{});

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_MPL_HPP_
