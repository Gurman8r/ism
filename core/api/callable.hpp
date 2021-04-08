#ifndef _ISM_CALLABLE_HPP_
#define _ISM_CALLABLE_HPP_

#include <core/api/runtime.hpp>

namespace ISM
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct Callable;

	ALIAS(CALLABLE) Ref<Callable>;

	struct Callable : public Reference
	{
	protected:
		Callable() : Reference{} {}

	public:
		virtual ~Callable() {}

		virtual void ptrcall(void * inst, void const ** args, void *& retv) const = 0;

		template <class R = void, class ... Args
		> auto call(void * inst, Args && ... args) const
		{
			void * retv{};

			if constexpr (constexpr size_t argc{ sizeof...(Args) }; argc == 0)
			{
				ptrcall(inst, NULL, retv);
			}
			else
			{
				void const * argv[argc]{};
				
				auto const capture{ std::forward_as_tuple(FWD(args)...) };
				
				Callable::collect_arguments(argv, &capture);
				
				ptrcall(inst, argv, retv);
			}

			if constexpr (!std::is_same_v<R, void>)
			{
				return ISM::make_caster<R>::convert(retv);
			}
		}

	private:
		template <class Capture
		> static void collect_arguments(void const ** v, Capture const * c) noexcept
		{
			Callable::impl_collect_arguments<Capture, 0, std::tuple_size_v<Capture>>(v, *c);
		}

		template <class Capture, size_t I, size_t N
		> static void impl_collect_arguments(void const ** v, Capture const & c)
		{
			static_assert(std::tuple_size_v<Capture> <= N);

			if constexpr (I < N)
			{
				using U = typename std::tuple_element_t<I, Capture>;

				ISM::make_caster<U>::encode(std::get<I>(c), const_cast<void *&>(v[I]));

				Callable::impl_collect_arguments<Capture, I + 1, N>(v, c);
			}
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// void(*)(P...)
	template <class ... P
	> struct Callable_V : Callable
	{
		static constexpr auto indices{ std::make_index_sequence<sizeof...(P)>{} };

		StdFn<void(P...)> impl;

		template <class F
		> Callable_V(F && value) : Callable{}, impl{ FWD(value) } {}

		virtual void ptrcall(void * inst, void const ** args, void *& retv) const override
		{
			ISM::ptrcall_v_helper<P...>(*this, args, indices);
		}
	};

	template <class ... P, size_t ... I
	> void ptrcall_v_helper(Callable_V<P...> const & call, void const ** args, std::index_sequence<I...>)
	{
		call.impl(ISM::make_caster<P>::convert(args[I])...);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// R(*)(P...)
	template <class R, class ... P
	> struct Callable_R : Callable
	{
		static constexpr auto indices{ std::make_index_sequence<sizeof...(P)>{} };

		StdFn<R(P...)> impl;

		template <class F
		> Callable_R(F && value) : Callable{}, impl{ FWD(value) } {}

		virtual void ptrcall(void * inst, void const ** args, void *& retv) const override
		{
			ISM::ptrcall_r_helper<R, P...>(*this, args, retv, indices);
		}
	};

	template <class R, class ... P, size_t ... I
	> void ptrcall_r_helper(Callable_R<R, P...> const & call, void const ** args, void *& retv, std::index_sequence<I...>)
	{
		ISM::make_caster<R>::encode(call.impl(ISM::make_caster<P>::convert(args[I])...), retv);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// void(T::*)(P...)
	template <class T, class ... P
	> struct Callable_T : Callable
	{
		static constexpr auto indices{ std::make_index_sequence<sizeof...(P)>{} };

		StdFn<void(T *, P...)> impl;

		template <class F
		> Callable_T(F && value) : Callable{}, impl{ FWD(value) } {}

		virtual void ptrcall(void * inst, void const ** args, void *& retv) const override
		{
			ISM::ptrcall_t_helper<T, P...>((T *)inst, *this, args, retv, indices);
		}
	};

	template <class T, class ... P, size_t ... I
	> void ptrcall_t_helper(T * inst, Callable_T<T, P...> const & call, void const ** args, void *& retv, std::index_sequence<I...>)
	{
		call.impl(inst, ISM::make_caster<P>::convert(args[I])...);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// void(T::*)(P...) const
	template <class T, class ... P
	> struct Callable_TC : Callable
	{
		static constexpr auto indices{ std::make_index_sequence<sizeof...(P)>{} };

		StdFn<void(T const *, P...)> impl;

		template <class F
		> Callable_TC(F && value) : Callable{}, impl{ FWD(value) } {}

		virtual void ptrcall(void * inst, void const ** args, void *& retv) const override
		{
			ISM::ptrcall_tc_helper<T, P...>((T const *)inst, *this, args, retv, indices);
		}
	};

	template <class T, class ... P, size_t ... I
	> void ptrcall_tc_helper(T const * inst, Callable_TC<T, P...> const & call, void const ** args, void *& retv, std::index_sequence<I...>)
	{
		call.impl(inst, ISM::make_caster<P>::convert(args[I])...);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// R(T::*)(P...)
	template <class T, class R, class ... P
	> struct Callable_TR : Callable
	{
		static constexpr auto indices{ std::make_index_sequence<sizeof...(P)>{} };

		StdFn<R(T *, P...)> impl;

		template <class F
		> Callable_TR(F && value) : Callable{}, impl{ FWD(value) } {}

		virtual void ptrcall(void * inst, void const ** args, void *& retv) const override
		{
			ISM::ptrcall_tr_helper<T, R, P...>((T *)inst, *this, args, retv, indices);
		}
	};

	template <class T, class R, class ... P, size_t ... I
	> void ptrcall_tr_helper(T * inst, Callable_TR<T, R, P...> const & call, void const ** args, void *& retv, std::index_sequence<I...>)
	{
		ISM::make_caster<R>::encode(call.impl(inst, ISM::make_caster<P>::convert(args[I])...), retv);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// R(T::*)(P...) const
	template <class T, class R, class ... P
	> struct Callable_TRC : Callable
	{
		static constexpr auto indices{ std::make_index_sequence<sizeof...(P)>{} };

		StdFn<R(T const *, P...)> impl;

		template <class F
		> Callable_TRC(F && value) : Callable{}, impl{ FWD(value) } {}

		virtual void ptrcall(void * inst, void const ** args, void *& retv) const override
		{
			ISM::ptrcall_trc_helper<T, R, P...>((T const *)inst, *this, args, retv, indices);
		}
	};

	template <class T, class R, class ... P, size_t ... I
	> void ptrcall_trc_helper(T const * inst, Callable_TRC<T, R, P...> const & call, void const ** args, void *& retv, std::index_sequence<I...>)
	{
		ISM::make_caster<R>::encode(call.impl(inst, ISM::make_caster<P>::convert(args[I])...), retv);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class R, class ... P
	> auto bind_method(R(*method)(P...)) noexcept
	{
		if constexpr (std::is_same_v<R, void>)
		{
			return CALLABLE{ memnew((Callable_V<P...>)(method)) };
		}
		else
		{
			return CALLABLE{ memnew((Callable_R<R, P...>)(method)) };
		}
	}

	template <class T, class R, class ... P
	> auto bind_method(R(T::*method)(P...)) noexcept
	{
		if constexpr (std::is_same_v<R, void>)
		{
			return CALLABLE{ memnew((Callable_T<T, P...>)(method)) };
		}
		else
		{
			return CALLABLE{ memnew((Callable_TR<T, R, P...>)(method)) };
		}
	}

	template <class T, class R, class ... P
	> auto bind_method(R(T::*method)(P...) const) noexcept
	{
		if constexpr (std::is_same_v<R, void>)
		{
			return CALLABLE{ memnew((Callable_TC<T, P...>)(method)) };
		}
		else
		{
			return CALLABLE{ memnew((Callable_TRC<T, R, P...>)(method)) };
		}
	}

	template <class R, class ... P
	> auto bind_method(StdFn<R(P...)> && method) noexcept
	{
		if constexpr (std::is_same_v<R, void>)
		{
			return CALLABLE{ memnew((Callable_V<P...>)(FWD(method))) };
		}
		else
		{
			return CALLABLE{ memnew((Callable_R<R, P...>)(FWD(method))) };
		}
	}

	template <class F, class ... P
	> auto bind_method(std::_Binder<std::_Unforced, F, P...> && method) noexcept
	{
		return ISM::bind_method(StdFn<void(P...)>(FWD(method)));
	}

	template <class R, class F, class ... P
	> auto bind_method(std::_Binder<R, F, P...> && method) noexcept
	{
		using U = std::_Binder_result_type<R, F>;
		return ISM::bind_method(StdFn<U(P...)>(FWD(method)));
	}

	template <class F, class ... Extra, class = std::enable_if_t<is_lambda_v<F>>
	> auto bind_method(F && method, Extra && ... extra) noexcept
	{
		return ISM::bind_method(std::bind(FWD(method), FWD(extra)...));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_CALLABLE_HPP_
