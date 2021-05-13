#ifndef _ISM_CALL_HPP_
#define _ISM_CALL_HPP_

#include <core/api/detail/cast.hpp>

namespace ism::detail
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <ReturnPolicy policy = ReturnPolicy_AutomaticReference
	> struct NODISCARD argument_collector
	{
		template <class ... Args
		> explicit argument_collector(Args && ... values)
		{
			m_args.reserve(sizeof...(Args));

			mpl::for_args([&](auto && e) noexcept
			{
				m_args.append(make_caster<decltype(e)>::cast(FWD(e), policy, nullptr));
			}
			, FWD(values)...);
		}

		NODISCARD LIST const & args() const & { return m_args; }

		NODISCARD LIST args() && { return std::move(m_args); }

		NODISCARD OBJECT call(OBJECT callable)
		{
			if (!callable || !m_args)
			{
				return nullptr;
			}
			else if (vectorcallfunc func{ get_vectorcall_func(callable) })
			{
				return func(callable, m_args.data(), m_args.size());
			}
			else if (TYPE t{ typeof(callable) }; t && t->tp_call)
			{
				return t->tp_call(callable, m_args);
			}
			else
			{
				return nullptr;
			}
		}

	private: LIST m_args{ CoreList{} };
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <ReturnPolicy policy = ReturnPolicy_AutomaticReference, class ... Args
	> NODISCARD auto collect_arguments(Args && ... args)
	{
		return argument_collector<policy>{ FWD(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args
	> struct NODISCARD argument_loader
	{
		static constexpr size_t argument_count{ sizeof...(Args) };

		static constexpr auto indices{ std::make_index_sequence<argument_count>() };

		std::tuple<make_caster<Args>...> argcasters{};

		NODISCARD bool load_args(Batch<OBJECT, bool> & args) { return impl_load_args(args, indices); }

		template <class Return, class Guard, class Func
		> NODISCARD auto call(Func && func) && -> std::conditional_t<std::is_void_v<Return>, void_type, Return>
		{
			if constexpr (std::is_void_v<Return>)
			{
				std::move(*this).call_impl<Return>(FWD(func), indices, Guard{});

				return void_type{};
			}
			else
			{
				return std::move(*this).call_impl<Return>(FWD(func), indices, Guard{});
			}
		}

	private:
		static bool impl_load_args(Batch<OBJECT, bool> &, std::index_sequence<>) noexcept { return true; }

		template <size_t ... I> bool impl_load_args(Batch<OBJECT, bool> & args, std::index_sequence<I...>)
		{
			return !(... || !std::get<I>(argcasters).load(args.get<OBJECT>(I), args.get<bool>(I)));
		}

		template <class Return, class Func, size_t ... I, class Guard
		> Return call_impl(Func && func, std::index_sequence<I...>, Guard &&) &&
		{
			return func(cast_op<Args>(std::move(std::get<I>(argcasters)))...);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct NODISCARD argument_record
	{
		String name{};

		OBJECT value{};

		bool convert{}, none{};

		DEFAULT_COPY_AND_MOVE_CONSTRUCTABLE(argument_record);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct NODISCARD function_record
	{
	public:
		String name{}, doc{}, signature{};

		OBJECT(*impl)(struct function_call & call) {};

		void * data[3]{};

		void(*free_data)(function_record * record) {};

		uint16_t argument_count{};

		Vector<argument_record> args{};

		ReturnPolicy policy{ ReturnPolicy_AutomaticReference };

		bool is_stateless{}, is_constructor{}, is_operator{}, is_method{}, prepend{};

		CoreObject * scope{}, * sibling{};

	public:
		~function_record() { if (free_data) { free_data(this); } }

		DEFAULT_COPY_AND_MOVE_CONSTRUCTABLE(function_record);

		template <class Return, class ... Args, class ... Extra
		> function_record(Return(*f)(Args...), Extra && ... extra)
		{
			this->initialize( f, f, FWD(extra)...);
		}

		template <class Func, class ... Extra, class = std::enable_if_t<mpl::is_lambda_v<Func>>
		> function_record(Func && f, Extra && ... extra)
		{
			this->initialize(FWD(f), (mpl::function_signature_t<Func> *)0, FWD(extra)...);
		}

		template <class Return, class Class, class ... Args, class ... Extra
		> function_record(Return(Class:: * f)(Args...), Extra && ... extra)
		{
			this->initialize([f](Class * c, Args ... args) -> Return { return (c->*f)(args...); }, (Return(*)(Class *, Args...))0, FWD(extra)...);
		}

		template <class Return, class Class, class ... Args, class ... Extra
		> function_record(Return(Class:: * f)(Args...) &, Extra && ... extra)
		{
			this->initialize([f](Class * c, Args ... args) -> Return { return (c->*f)(args...); }, (Return(*)(Class *, Args...))0, FWD(extra)...);
		}

		template <class Return, class Class, class ... Args, class ... Extra
		> function_record(Return(Class:: * f)(Args...) const, Extra && ... extra)
		{
			this->initialize([f](Class const * c, Args ... args) -> Return { return (c->*f)(args...); }, (Return(*)(Class const *, Args...))0, FWD(extra)...);
		}

		template <class Return, class Class, class ... Args, class ... Extra
		> function_record(Return(Class:: * f)(Args...) const &, Extra && ... extra)
		{
			this->initialize([f](Class const * c, Args ... args) -> Return { return (c->*f)(args...); }, (Return(*)(Class const *, Args...))0, FWD(extra)...);
		}

	protected:
		template <class Func, class Return, class ... Args, class ... Extra
		> void initialize(Func && func, Return(*)(Args...), Extra && ... extra)
		{
			struct Capture { std::remove_reference_t<Func> value; };

			if (sizeof(Capture) <= sizeof(this->data))
			{
				::new((Capture *)&this->data) Capture{ FWD(func) };

				if constexpr (!std::is_trivially_destructible_v<Func>)
				{
					this->free_data = [](function_record * record) { ((Capture *)&record->data)->~Capture(); };
				}
			}
			else
			{
				this->data[0] = memnew(Capture{ FWD(func) });

				this->free_data = [](function_record * record) { memdelete((Capture *)record->data[0]); };
			}

			// convert function arguments and perform the actual function call
			this->impl = [](function_call & call) -> OBJECT
			{
				argument_loader<Args...> args{};
				if (!args.load_args(call.args)) { return nullptr; }

				process_attributes<Extra...>::precall(call);

				auto data{ (sizeof(Capture) <= sizeof(call.func.data) ? &call.func.data : call.func.data[0]) };

				auto capture{ const_cast<Capture *>(reinterpret_cast<Capture const *>(data)) };

				ReturnPolicy policy{ return_policy_override<Return>::policy(call.func.policy) };

				using Guard = extract_guard_t<Extra...>;

				using Yield = make_caster<std::conditional_t<std::is_void_v<Return>, void_type, Return>>;

				OBJECT result{ Yield::cast(std::move(args).call<Return, Guard>(capture->value), policy, call.parent) };

				process_attributes<Extra...>::postcall(call, result);

				return result;
			};

			// process function attributes
			process_attributes<Extra...>::init(*this, FWD(extra)...);

			// generate type info
			constexpr size_t argc{ sizeof...(Args) };
			Array<std::type_info const *, argc> types{};
			mpl::for_types_i<Args...>([&](size_t i, auto tag) noexcept
			{
				types[i] = &typeid(TAG_TYPE(tag));
			});

			// initialize generic
			this->initialize_generic(types, argc);

			// stash some additional information used by an important optimization in 'functional.h'
			if constexpr (std::is_convertible_v<Func, Return(*)(Args...)> && sizeof(Capture) == sizeof(void *))
			{
				this->is_stateless = true;
				this->data[1] = const_cast<void *>(reinterpret_cast<void const *>(&typeid(Return(*)(Args...))));
			}
		}

		void initialize_generic(std::type_info const * const * info_in, size_t argc_in)
		{
			this->argument_count = argc_in;

			this->args.reserve(argc_in);

			for (size_t i = 0; i < argc_in; ++i)
			{
				String const arg_str{ "arg" + util::to_string(i) };

				this->args.push_back({ arg_str, nullptr, false, false });
			}
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct NODISCARD function_call
	{
	public:
		function_record const & func;
		
		OBJECT parent{};

		Batch<OBJECT, bool> args{ func.argument_count };

	public:
		function_call & operator()(OBJECT const * argv, size_t argc)
		{
			size_t
				num_args	{ this->func.argument_count },
				num_to_copy	{ MIN(num_args, argc) },
				num_copied	{};

			// copy passed arguments
			for (; num_copied < num_to_copy; ++num_copied)
			{
				argument_record const * arg_rec{ num_copied < func.args.size() ? &func.args[num_copied] : nullptr };

				OBJECT arg{ argv[num_copied] };

				VERIFY("BAD ARGUMENT" && !(arg_rec && !arg_rec->none && arg.is_null()));

				this->args.push_back(arg, arg_rec ? arg_rec->convert : false);
			}

			// fill in missing arguments
			if (num_copied < num_args)
			{
				for (; num_copied < num_args; ++num_copied)
				{
					if (auto const & arg{ func.args[num_copied] }; arg.value)
					{
						this->args.push_back(arg.value, arg.convert);
					}
					else
					{
						break;
					}
				}
				VERIFY("NOT ENOUGH ARGUMENTS" && (num_args <= num_copied));
			}

			return (*this);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class /*Derived*/, class Func
	> auto method_adaptor(Func && func) -> decltype(std::forward<Func>(func)) { return std::forward<Func>(func); }

	template <class Derived, class Return, class Class, class ... Args
	> auto method_adaptor(Return(Class:: * pmf)(Args...))->Return(Derived:: *)(Args...)
	{
		static_assert(
			is_accessible_base_of_v<Class, Derived>,
			"Cannot bind an inaccessible base class method; use a lambda definition instead");
		return pmf;
	}

	template <class Derived, class Return, class Class, class ... Args
	> auto method_adaptor(Return(Class:: * pmf)(Args...) const)->Return(Derived:: *)(Args...) const
	{
		static_assert(
			is_accessible_base_of_v<Class, Derived>,
			"Cannot bind an inaccessible base class method; use a lambda definition instead");
		return pmf;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Derived
	> template <ReturnPolicy policy, class ...Args
	> inline OBJECT ObjectAPI<Derived>::operator()(Args && ... args) const
	{
		return detail::collect_arguments<policy>(FWD(args)...).call(handle());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_CALL_HPP_
