#ifndef _ISM_CALLABLE_HPP_
#define _ISM_CALLABLE_HPP_

#include <core/api/reference.hpp>
#include <core/templates/functional.hpp>
#include <core/templates/mpl.hpp>

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

			if constexpr (constexpr size_t argc{ sizeof...(Args) }; 0 < argc)
			{
				void const * argv[argc]{};

				auto const capture{ std::forward_as_tuple(FWD(args)...) };

				Callable::collect_arguments(argv, &capture);

				ptrcall(inst, argv, retv);
			}
			else
			{
				ptrcall(inst, NULL, retv);
			}

			if constexpr (!std::is_same_v<R, void>)
			{
				return ISM::object_convert<R>(retv);
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
			if constexpr (I < N)
			{
				using U = typename std::tuple_element_t<I, Capture>;

				ISM::object_encode<U>(std::get<I>(c), const_cast<void *&>(v[I]));

				Callable::impl_collect_arguments<Capture, I + 1, N>(v, c);
			}
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// void(*)(P...)
	template <class ... P
	> struct Callable_V : Callable
	{
		static constexpr size_t nargs{ sizeof...(P) };

		static constexpr auto indices{ std::make_index_sequence<nargs>() };

		StdFn<void(P...)> impl;

		template <class F
		> Callable_V(F && value) : Callable{}, impl{ FWD(value) } {}

		virtual void ptrcall(void * inst, void const ** args, void *& retv) const override
		{
			this->expand_ptrcall(args, indices);
		}

		template <size_t ... I
		> void expand_ptrcall(void const ** args, std::index_sequence<I...>) const
		{
			impl(ISM::object_convert<P>(args[I])...);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// R(*)(P...)
	template <class R, class ... P
	> struct Callable_R : Callable
	{
		static constexpr size_t nargs{ sizeof...(P) };

		static constexpr auto indices{ std::make_index_sequence<nargs>() };

		StdFn<R(P...)> impl;

		template <class F
		> Callable_R(F && value) : Callable{}, impl{ FWD(value) } {}

		virtual void ptrcall(void * inst, void const ** args, void *& retv) const override
		{
			this->expand_ptrcall(args, retv, indices);
		}

		template <size_t ... I
		> void expand_ptrcall(void const ** args, void *& retv, std::index_sequence<I...>) const
		{
			ISM::object_encode<R>(impl(ISM::object_convert<P>(args[I])...), retv);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// void(T::*)(P...)
	template <class T, class ... P
	> struct Callable_T : Callable
	{
		static constexpr size_t nargs{ sizeof...(P) };

		static constexpr auto indices{ std::make_index_sequence<nargs>() };

		StdFn<void(T *, P...)> impl;

		template <class F
		> Callable_T(F && value) : Callable{}, impl{ FWD(value) } {}

		virtual void ptrcall(void * inst, void const ** args, void *& retv) const override
		{
			this->expand_ptrcall((T *)inst, args, retv, indices);
		}

		template <size_t ... I
		> void expand_ptrcall(T * inst, void const ** args, void *& retv, std::index_sequence<I...>) const
		{
			impl(inst, ISM::object_convert<P>(args[I])...);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// void(T::*)(P...) const
	template <class T, class ... P
	> struct Callable_TC : Callable
	{
		static constexpr size_t nargs{ sizeof...(P) };

		static constexpr auto indices{ std::make_index_sequence<nargs>() };

		StdFn<void(T const *, P...)> impl;

		template <class F
		> Callable_TC(F && value) : Callable{}, impl{ FWD(value) } {}

		virtual void ptrcall(void * inst, void const ** args, void *& retv) const override
		{
			this->expand_ptrcall((T const *)inst, args, retv, indices);
		}

		template <size_t ... I
		> void expand_ptrcall(T const * inst, void const ** args, void *& retv, std::index_sequence<I...>) const
		{
			impl(inst, ISM::object_convert<P>(args[I])...);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// R(T::*)(P...)
	template <class T, class R, class ... P
	> struct Callable_TR : Callable
	{
		static constexpr size_t nargs{ sizeof...(P) };

		static constexpr auto indices{ std::make_index_sequence<nargs>() };

		StdFn<R(T *, P...)> impl;

		template <class F
		> Callable_TR(F && value) : Callable{}, impl{ FWD(value) } {}

		virtual void ptrcall(void * inst, void const ** args, void *& retv) const override
		{
			this->expand_ptrcall((T *)inst, args, retv, indices);
		}

		template <size_t ... I
		> void expand_ptrcall(T * inst, void const ** args, void *& retv, std::index_sequence<I...>) const
		{
			ISM::object_encode<R>(impl(inst, ISM::object_convert<P>(args[I])...), retv);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// R(T::*)(P...) const
	template <class T, class R, class ... P
	> struct Callable_TRC : Callable
	{
		static constexpr size_t nargs{ sizeof...(P) };

		static constexpr auto indices{ std::make_index_sequence<nargs>() };

		StdFn<R(T const *, P...)> impl;

		template <class F
		> Callable_TRC(F && value) : Callable{}, impl{ FWD(value) } {}

		virtual void ptrcall(void * inst, void const ** args, void *& retv) const override
		{
			this->expand_ptrcall((T const *)inst, args, retv, indices);
		}

		template <size_t ... I
		> void expand_ptrcall(T const * inst, void const ** args, void *& retv, std::index_sequence<I...>) const
		{
			ISM::object_encode<R>(impl(inst, ISM::object_convert<P>(args[I])...), retv);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class R, class ... P
	> auto bind_method(R(*f)(P...)) noexcept
	{
		if constexpr (std::is_same_v<R, void>)
		{
			return CALLABLE{ memnew((Callable_V<P...>)(f)) };
		}
		else
		{
			return CALLABLE{ memnew((Callable_R<R, P...>)(f)) };
		}
	}

	template <class T, class R, class ... P
	> auto bind_method(R(T::*f)(P...)) noexcept
	{
		if constexpr (std::is_same_v<R, void>)
		{
			return CALLABLE{ memnew((Callable_T<T, P...>)(f)) };
		}
		else
		{
			return CALLABLE{ memnew((Callable_TR<T, R, P...>)(f)) };
		}
	}

	template <class T, class R, class ... P
	> auto bind_method(R(T::*f)(P...) const) noexcept
	{
		if constexpr (std::is_same_v<R, void>)
		{
			return CALLABLE{ memnew((Callable_TC<T, P...>)(f)) };
		}
		else
		{
			return CALLABLE{ memnew((Callable_TRC<T, R, P...>)(f)) };
		}
	}

	template <class R, class ... P
	> auto bind_method(StdFn<R(P...)> && f) noexcept
	{
		if constexpr (std::is_same_v<R, void>)
		{
			return CALLABLE{ memnew((Callable_V<P...>)(FWD(f))) };
		}
		else
		{
			return CALLABLE{ memnew((Callable_R<R, P...>)(FWD(f))) };
		}
	}

	template <class F, class ... P
	> auto bind_method(std::_Binder<std::_Unforced, F, P...> && f) noexcept
	{
		return ISM::bind_method(StdFn<void(P...)>(FWD(f)));
	}

	template <class R, class F, class ... P
	> auto bind_method(std::_Binder<R, F, P...> && f) noexcept
	{
		using U = std::_Binder_result_type<R, F>;
		return ISM::bind_method(StdFn<U(P...)>(FWD(f)));
	}

	template <class F, class ... Extra, class = std::enable_if_t<is_lambda_v<F>>
	> auto bind_method(F && f, Extra && ... extra) noexcept
	{
		return ISM::bind_method(std::bind(FWD(f), FWD(extra)...));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_CALLABLE_HPP_
