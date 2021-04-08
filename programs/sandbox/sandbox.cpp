#include <main/main.hpp>
#include <core/api/callable.hpp>

using namespace ISM;
extern OS const * create_os(void * obj = {});
static OS const * ANONYMOUS{ CHECK(create_os()) };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define MAIN_PRINT (ISM::get_os().print)

#if ISM_OS_WINDOWS
#define MAIN_PAUSE() \
	std::system("pause")
#else
#define MAIN_PAUSE() \
	([]() noexcept -> int { \
		ISM::MAIN_PRINT("Press enter to continue . . ."); \
		ISM::get_os().get_stdin_string(true); \
		return 0; \
	})()
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ISM
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct Callable : public Reference
	{
	protected:
		Callable() : Reference{} {}

	public:
		virtual ~Callable() {}

		virtual void ptrcall(void * obj, void const ** args, void * out) const = 0;

		template <class R = void, class ... P
		> R call(void * o, P && ... p) const
		{
			constexpr auto N{ sizeof...(P) };

			void * result{};

			if constexpr (0 < N)
			{
				void const * args[N]{};

				MPL::array<Any, N> tpl{ std::forward_as_tuple(FWD(p)...) };

				MPL::for_tuple_i(tpl, [&](size_t i, Any const & e)
				{
					args[i] = &e;
				});

				ptrcall(o, args, result);
			}
			else
			{
				ptrcall(o, 0, result);
			}

			if constexpr (!std::is_same_v<R, void>)
			{
				return PtrToArg<R>::convert(result);
			}
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... P, size_t ... I
	> void call_ptr_args_v_helper(StdFn<void(P...)> const & method, void const ** args, std::index_sequence<I...>)
	{
		(method)(PtrToArg<P>::convert(args[I])...);
	}

	template <class ... P
	> struct Callable_V : Callable
	{
		StdFn<void(P...)> method;

		template <class F
		> Callable_V(F && value) : Callable{}, method{ FWD(value) } {}

		virtual void ptrcall(void * obj, void const ** args, void * out) const override
		{
			ISM::call_ptr_args_v_helper<P...>(method, args, std::make_index_sequence<sizeof...(P)>{});
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class R, class ... P, size_t ... I
	> void call_ptr_args_r_helper(StdFn<R(P...)> const & method, void const ** args, void * out, std::index_sequence<I...>)
	{
		PtrToArg<R>::encode(method(PtrToArg<P>::convert(args[I])...), out);
	}

	template <class R, class ... P
	> struct Callable_R : Callable
	{
		StdFn<R(P...)> method;

		template <class F
		> Callable_R(F && value) : Callable{}, method{ FWD(value) } {}

		virtual void ptrcall(void * obj, void const ** args, void * out) const override
		{
			ISM::call_ptr_args_r_helper<R, P...>(method, args, out, std::make_index_sequence<sizeof...(P)>{});
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class ... P, size_t ... I
	> void call_ptr_args_t_helper(T * obj, StdFn<void(T *, P...)> const & method, void const ** args, void * out, std::index_sequence<I...>)
	{
		std::invoke(method, obj, PtrToArg<P>::convert(args[I])...);
	}

	template <class T, class ... P
	> struct Callable_T : Callable
	{
		StdFn<void(T *, P...)> method;

		template <class F
		> Callable_T(F && value) : Callable{}, method{ FWD(value) } {}

		virtual void ptrcall(void * obj, void const ** args, void * out) const override
		{
			ISM::call_ptr_args_t_helper<T, P...>((T *)obj, method, args, out, std::make_index_sequence<sizeof...(P)>{});
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class ... P, size_t ... I
	> void call_ptr_args_tc_helper(T const * obj, StdFn<void(T const *, P...)> const & method, void const ** args, void * out, std::index_sequence<I...>)
	{
		std::invoke(method, obj, PtrToArg<P>::convert(args[I])...);
	}

	template <class T, class ... P
	> struct Callable_TC : Callable
	{
		StdFn<void(T const *, P...)> method;

		template <class F
		> Callable_TC(F && value) : Callable{}, method{ FWD(value) } {}

		virtual void ptrcall(void * obj, void const ** args, void * out) const override
		{
			ISM::call_ptr_args_tc_helper<T, P...>((T const *)obj, method, args, out, std::make_index_sequence<sizeof...(P)>{});
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class R, class ... P, size_t ... I
	> void call_ptr_args_tr_helper(T * obj, StdFn<R(T *, P...)> const & method, void const ** args, void * out, std::index_sequence<I...>)
	{
		PtrToArg<R>::encode(std::invoke(method, obj, PtrToArg<P>::convert(args[I])...), out);
	}

	template <class T, class R, class ... P
	> struct Callable_TR : Callable
	{
		StdFn<R(T *, P...)> method;

		template <class F
		> Callable_TR(F && value) : Callable{}, method{ FWD(value) } {}

		virtual void ptrcall(void * obj, void const ** args, void * out) const override
		{
			ISM::call_ptr_args_tr_helper<T, R, P...>((T *)obj, method, args, out, std::make_index_sequence<sizeof...(P)>{});
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class R, class ... P, size_t ... I
	> void call_ptr_args_trc_helper(T const * obj, StdFn<R(T const *, P...)> const & method, void const ** args, void * out, std::index_sequence<I...>)
	{
		PtrToArg<R>::encode(std::invoke(method, obj, PtrToArg<P>::convert(args[I])...), out);
	}

	template <class T, class R, class ... P
	> struct Callable_TRC : Callable
	{
		StdFn<R(T const *, P...)> method;

		template <class F
		> Callable_TRC(F && value) : Callable{}, method{ FWD(value) } {}

		virtual void ptrcall(void * obj, void const ** args, void * out) const override
		{
			ISM::call_ptr_args_trc_helper<T, R, P...>((T const *)obj, method, args, out, std::make_index_sequence<sizeof...(P)>{});
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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ISM
{
	struct Test
	{
		static void test_static() { MAIN_PRINT("%s\n", PRETTY_FUNCTION); }

		auto test_const() const { MAIN_PRINT("%s\n", PRETTY_FUNCTION); }
	};

	void say_hello() { printf("Hello, World!\n"); }

	void pass_int(int i) { printf("%d\n", i); }

	int get_int() { return 123; }

	void say(String const & s) { printf("%.*s\n", s.size(), s.data()); }

	Err test_main(int32_t argc, char * argv[])
	{
		bind_method(say_hello)->call(NULL);
		bind_method([&]() { printf("Hello, World!\n"); })->call(NULL);
		bind_method(say)->call(NULL, String{ "Goodbye, World!" });

		Test test{};
		bind_method(&Test::test_static)->call(NULL);
		bind_method(&Test::test_const)->call(&test);

		return Err_None;

		OBJECT o{ make_generic(DICT::type_static()) };
		o["ABC"] = 42;
		o["DEF"] = "Hello, World!";

		MAIN_PRINT("%d\n", o["ABC"].cast<int>());
		MAIN_PRINT("%s\n", o["DEF"].cast<String>().c_str());

		MAIN_PRINT("%s\n", o.type().attr("__name__").cast<std::string>().c_str());

		o.type().attr("__name__") = "changed";

		MAIN_PRINT("%s\n", STR(o.type().attr("__name__"))->c_str());

		return Err_None;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int main(int argc, char * argv[])
{
	switch (Main::setup(argv[0], argc, argv))
	{
	case Err_None: break;
	default: FATAL("An unknown error occurred during setup and the program was unable to start.");
	}

	VERIFY(Main::start());

	Err result{ test_main(argc, argv) };

	Main::cleanup();

	VERIFY(result == Err_None);

	return MAIN_PAUSE();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */