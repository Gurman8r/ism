#ifndef _ISM_CALLABLE_HPP_
#define _ISM_CALLABLE_HPP_

#include <core/api/runtime.hpp>

namespace ISM
{
	template <class T
	> struct PtrToArg {};

#define MAKE_PTRARG(T)									\
	template <> struct ISM::PtrToArg<T>					\
	{													\
		static bool convert(void const * in)			\
		{												\
			return *reinterpret_cast<T const *>(in);	\
		}												\
		static void encode(T in, void * out)			\
		{												\
			*((T *)out) = in;							\
		}												\
	}

#define MAKE_PTRARG_CONVT(To, From)											\
	template <> struct ISM::PtrToArg<To>									\
	{																		\
		static To convert(void const * in)									\
		{																	\
			return static_cast<To>(*reinterpret_cast<From const *>(in));	\
		}																	\
		static void encode(To in, void * out)								\
		{																	\
			*((From *)out) = static_cast<From>(in);							\
		}																	\
	};																		\
	template <> struct ISM::PtrToArg<To const &>							\
	{																		\
		static To convert(void const * in)									\
		{																	\
			return static_cast<To>(*reinterpret_cast<From const *>(in));	\
		}																	\
																			\
		static void encode(To in, void * out)								\
		{																	\
			*((From *)out) = static_cast<From>(in);							\
		}																	\
	}

#define MAKE_PTRARG_REFERENCE(T)						\
	template <> struct ISM::PtrToArg<T>					\
	{													\
		static T convert(void const * in)				\
		{												\
			return *reinterpret_cast<T const *>(in);	\
		}												\
		static void encode(T const & in, void * out)	\
		{												\
			*((T *)out) = in;							\
		}												\
	};													\
	template <> struct ISM::PtrToArg<T const &>			\
	{													\
		static T convert(void const * in)				\
		{												\
			return *reinterpret_cast<T const *>(in);	\
		}												\
		static void encode(T const & in, void * out)	\
		{												\
			*((T *)out) = in;							\
		}												\
	}

	MAKE_PTRARG(bool);
	MAKE_PTRARG(char);

	MAKE_PTRARG_CONVT(int8_t, int64_t);
	MAKE_PTRARG_CONVT(int16_t, int64_t);
	MAKE_PTRARG_CONVT(int32_t, int64_t);
	MAKE_PTRARG_CONVT(uint8_t, uint64_t);
	MAKE_PTRARG_CONVT(uint16_t, uint64_t);
	MAKE_PTRARG_CONVT(uint32_t, uint64_t);
	MAKE_PTRARG(int64_t);
	MAKE_PTRARG(uint64_t);

	MAKE_PTRARG_CONVT(float32_t, float64_t);
	MAKE_PTRARG_CONVT(float80_t, float64_t);
	MAKE_PTRARG(float64_t);

	MAKE_PTRARG_REFERENCE(String);
}

namespace ISM
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct Callable : public Reference
	{
	protected:
		Callable() : Reference{} {}

	public:
		virtual ~Callable() {}

		virtual void ptrcall(void * self, void const ** args, void *& out) const = 0;

		template <class R = void, class ... Args
		> auto call(void * self, Args && ... args) const
		{
			void * out{};

			if constexpr (constexpr size_t argc{ sizeof...(Args) }; 0 < argc)
			{
				void const * argv[argc]{};

				MPL::array<Any, argc> capture{ std::forward_as_tuple(FWD(args)...) };
				
				MPL::for_tuple_i(capture, [&argv](size_t i, Any const & e)
				{
					argv[i] = std::addressof(e);
				});

				ptrcall(self, argv, out);
			}
			else
			{
				ptrcall(self, NULL, out);
			}

			if constexpr (!std::is_same_v<R, void>)
			{
				return PtrToArg<R>::convert(out);
			}
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... P, size_t ... I
	> void ptrcall_v_helper(StdFn<void(P...)> const & method, void const ** args, std::index_sequence<I...>)
	{
		method(PtrToArg<P>::convert(args[I])...);
	}

	template <class ... P
	> struct Callable_V : Callable
	{
		static constexpr auto indices{ std::make_index_sequence<sizeof...(P)>{} };

		StdFn<void(P...)> impl;

		template <class F
		> Callable_V(F && value) : Callable{}, impl{ FWD(value) } {}

		virtual void ptrcall(void * self, void const ** args, void *& out) const override
		{
			ISM::ptrcall_v_helper<P...>(impl, args, indices);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class R, class ... P, size_t ... I
	> void ptrcall_r_helper(StdFn<R(P...)> const & method, void const ** args, void * out, std::index_sequence<I...>)
	{
		PtrToArg<R>::encode(method(PtrToArg<P>::convert(args[I])...), out);
	}

	template <class R, class ... P
	> struct Callable_R : Callable
	{
		static constexpr auto indices{ std::make_index_sequence<sizeof...(P)>{} };

		StdFn<R(P...)> impl;

		template <class F
		> Callable_R(F && value) : Callable{}, impl{ FWD(value) } {}

		virtual void ptrcall(void * self, void const ** args, void *& out) const override
		{
			ISM::ptrcall_r_helper<R, P...>(impl, args, out, indices);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class ... P, size_t ... I
	> void ptrcall_t_helper(T * obj, StdFn<void(T *, P...)> const & method, void const ** args, void * out, std::index_sequence<I...>)
	{
		method(obj, PtrToArg<P>::convert(args[I])...);
	}

	template <class T, class ... P
	> struct Callable_T : Callable
	{
		static constexpr auto indices{ std::make_index_sequence<sizeof...(P)>{} };

		StdFn<void(T *, P...)> impl;

		template <class F
		> Callable_T(F && value) : Callable{}, impl{ FWD(value) } {}

		virtual void ptrcall(void * self, void const ** args, void *& out) const override
		{
			ISM::ptrcall_t_helper<T, P...>((T *)self, impl, args, out, indices);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class ... P, size_t ... I
	> void ptrcall_tc_helper(T const * obj, StdFn<void(T const *, P...)> const & method, void const ** args, void * out, std::index_sequence<I...>)
	{
		method(obj, PtrToArg<P>::convert(args[I])...);
	}

	template <class T, class ... P
	> struct Callable_TC : Callable
	{
		static constexpr auto indices{ std::make_index_sequence<sizeof...(P)>{} };

		StdFn<void(T const *, P...)> impl;

		template <class F
		> Callable_TC(F && value) : Callable{}, impl{ FWD(value) } {}

		virtual void ptrcall(void * self, void const ** args, void *& out) const override
		{
			ISM::ptrcall_tc_helper<T, P...>((T const *)self, impl, args, out, indices);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class R, class ... P, size_t ... I
	> void ptrcall_tr_helper(T * obj, StdFn<R(T *, P...)> const & method, void const ** args, void * out, std::index_sequence<I...>)
	{
		PtrToArg<R>::encode(method(obj, PtrToArg<P>::convert(args[I])...), out);
	}

	template <class T, class R, class ... P
	> struct Callable_TR : Callable
	{
		static constexpr auto indices{ std::make_index_sequence<sizeof...(P)>{} };

		StdFn<R(T *, P...)> impl;

		template <class F
		> Callable_TR(F && value) : Callable{}, impl{ FWD(value) } {}

		virtual void ptrcall(void * self, void const ** args, void *& out) const override
		{
			ISM::ptrcall_tr_helper<T, R, P...>((T *)self, impl, args, out, indices);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class R, class ... P, size_t ... I
	> void ptrcall_trc_helper(T const * obj, StdFn<R(T const *, P...)> const & method, void const ** args, void * out, std::index_sequence<I...>)
	{
		PtrToArg<R>::encode(method(obj, PtrToArg<P>::convert(args[I])...), out);
	}

	template <class T, class R, class ... P
	> struct Callable_TRC : Callable
	{
		static constexpr auto indices{ std::make_index_sequence<sizeof...(P)>{} };

		StdFn<R(T const *, P...)> impl;

		template <class F
		> Callable_TRC(F && value) : Callable{}, impl{ FWD(value) } {}

		virtual void ptrcall(void * self, void const ** args, void *& out) const override
		{
			ISM::ptrcall_trc_helper<T, R, P...>((T const *)self, impl, args, out, indices);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class R, class ... P
	> auto bind_method(R(*method)(P...)) noexcept
	{
		if constexpr (std::is_same_v<R, void>)
		{
			return Ref<Callable>{ memnew((Callable_V<P...>)(method)) };
		}
		else
		{
			return Ref<Callable>{ memnew((Callable_R<R, P...>)(method)) };
		}
	}

	template <class T, class R, class ... P
	> auto bind_method(R(T::*method)(P...)) noexcept
	{
		if constexpr (std::is_same_v<R, void>)
		{
			return Ref<Callable>{ memnew((Callable_T<T, P...>)(method)) };
		}
		else
		{
			return Ref<Callable>{ memnew((Callable_TR<T, R, P...>)(method)) };
		}
	}

	template <class T, class R, class ... P
	> auto bind_method(R(T::*method)(P...) const) noexcept
	{
		if constexpr (std::is_same_v<R, void>)
		{
			return Ref<Callable>{ memnew((Callable_TC<T, P...>)(method)) };
		}
		else
		{
			return Ref<Callable>{ memnew((Callable_TRC<T, R, P...>)(method)) };
		}
	}

	template <class R, class ... P
	> auto bind_method(StdFn<R(P...)> && method) noexcept
	{
		if constexpr (std::is_same_v<R, void>)
		{
			return Ref<Callable>{ memnew((Callable_V<P...>)(FWD(method))) };
		}
		else
		{
			return Ref<Callable>{ memnew((Callable_R<R, P...>)(FWD(method))) };
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
