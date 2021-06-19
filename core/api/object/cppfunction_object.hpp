#ifndef _ISM_CPPFUNCTION_OBJECT_HPP_
#define _ISM_CPPFUNCTION_OBJECT_HPP_

#include <core/api/initimpl.hpp>

// cppfunction object
namespace ism::api
{
	class ISM_API CppFunctionObject : public FunctionObject
	{
		ISM_OBJECT_CVT(CppFunctionObject, FunctionObject);

	protected:
		static void _bind_methods(TypeObject & t);

	public:
		function_record m_func{};

		vectorcallfunc m_vectorcall{};

		virtual ~CppFunctionObject() override { if (m_func.free_data) { m_func.free_data(&m_func); } }

		CppFunctionObject() noexcept : base_type{ get_type_static() } { m_vectorcall = dispatcher; }

		template <class Return, class ... Args, class ... Extra
		> CppFunctionObject(Return(*f)(Args...), Extra && ... extra) : self_type{}
		{
			initialize(f, f, FWD(extra)...);
		}

		template <class Func, class ... Extra, class = std::enable_if_t<mpl::is_lambda_v<Func>>
		> CppFunctionObject(Func && f, Extra && ... extra) : self_type{}
		{
			initialize(FWD(f), (mpl::function_signature_t<Func> *)0, FWD(extra)...);
		}

		template <class Return, class Class, class ... Args, class ... Extra
		> CppFunctionObject(Return(Class:: * f)(Args...), Extra && ... extra) : self_type{}
		{
			initialize([f](Class * c, Args ... args) -> Return { return (c->*f)(args...); }, (Return(*)(Class *, Args...))0, FWD(extra)...);
		}

		template <class Return, class Class, class ... Args, class ... Extra
		> CppFunctionObject(Return(Class:: * f)(Args...) &, Extra && ... extra) : self_type{}
		{
			initialize([f](Class * c, Args ... args) -> Return { return (c->*f)(args...); }, (Return(*)(Class *, Args...))0, FWD(extra)...);
		}

		template <class Return, class Class, class ... Args, class ... Extra
		> CppFunctionObject(Return(Class:: * f)(Args...) const, Extra && ... extra) : self_type{}
		{
			initialize([f](Class const * c, Args ... args) -> Return { return (c->*f)(args...); }, (Return(*)(Class const *, Args...))0, FWD(extra)...);
		}

		template <class Return, class Class, class ... Args, class ... Extra
		> CppFunctionObject(Return(Class:: * f)(Args...) const &, Extra && ... extra) : self_type{}
		{
			initialize([f](Class const * c, Args ... args) -> Return { return (c->*f)(args...); }, (Return(*)(Class const *, Args...))0, FWD(extra)...);
		}

	protected:
		template <class Func, class Return, class ... Args, class ... Extra
		> void initialize(Func && func, Return(*)(Args...), Extra && ... extra)
		{
			struct Capture { std::remove_reference_t<Func> value; };

			if (sizeof(Capture) <= sizeof(m_func.data))
			{
				::new((Capture *)&m_func.data) Capture{ FWD(func) };

				if constexpr (!std::is_trivially_destructible_v<Func>)
				{
					m_func.free_data = [](function_record * f) { ((Capture *)&f->data)->~Capture(); };
				}
			}
			else
			{
				m_func.data[0] = memnew(Capture{ FWD(func) });

				m_func.free_data = [](function_record * f) { memdelete((Capture *)f->data[0]); };
			}

			// convert function arguments and perform the actual function call
			m_func.impl = [](function_call & call) -> OBJECT
			{
				argument_loader<Args...> args{};
				if (!args.load_args(call.args)) { return nullptr; }

				attr::process_attributes<Extra...>::precall(call);

				auto data{ (sizeof(Capture) <= sizeof(call.func.data) ? &call.func.data : call.func.data[0]) };

				auto capture{ const_cast<Capture *>(reinterpret_cast<Capture const *>(data)) };

				ReturnPolicy policy{ return_policy_override<Return>::policy(call.func.policy) };

				using Guard = attr::extract_guard_t<Extra...>;

				using Yield = make_caster<std::conditional_t<std::is_void_v<Return>, void_type, Return>>;

				OBJECT result{ Yield::cast(std::move(args).call<Return, Guard>(capture->value), policy, call.parent) };

				attr::process_attributes<Extra...>::postcall(call, result);

				return result;
			};

			// process function attributes
			attr::process_attributes<Extra...>::init(m_func, FWD(extra)...);

			// generate type info
			constexpr size_t argc{ sizeof...(Args) };
			Array<std::type_info const *, argc> types{};
			mpl::for_types_i<Args...>([&](size_t i, auto tag) noexcept
			{
				types[i] = &typeid(TAG_TYPE(tag));
			});

			// initialize generic
			initialize_generic(types, argc);

			// stash some additional information used by an important optimization in 'functional.h'
			if constexpr (std::is_convertible_v<Func, Return(*)(Args...)> && sizeof(Capture) == sizeof(void *))
			{
				m_func.is_stateless = true;
				m_func.data[1] = const_cast<void *>(reinterpret_cast<void const *>(&typeid(Return(*)(Args...))));
			}
		}

		void initialize_generic(std::type_info const * const * info_in, size_t argc_in)
		{
			m_func.argument_count = argc_in;

			m_func.args.reserve(argc_in);

			for (size_t i = 0; i < argc_in; ++i)
			{
				// placeholder argument info

				String const arg_str{ "arg" + util::to_string(i) };

				m_func.args.push_back({ arg_str, nullptr, false, false });
			}
		}

		static OBJECT dispatcher(OBJECT callable, OBJECT const * argv, size_t argc)
		{
			if (!callable) { return nullptr; }

			function_record const & func{ super_cast<self_type>(*callable)->m_func };

			function_call call{ func, (0 < argc ? argv[0] : nullptr) };

			size_t
				num_args	{ func.argument_count },
				num_to_copy	{ MIN(num_args, argc) },
				num_copied	{};

			// copy passed arguments
			for (; num_copied < num_to_copy; ++num_copied)
			{
				argument_record const * arg_rec{ num_copied < func.args.size() ? &func.args[num_copied] : nullptr };

				OBJECT arg{ argv[num_copied] };

				VERIFY("BAD ARGUMENT" && !(arg_rec && !arg_rec->none && arg.is_null()));

				call.args.push_back(arg, arg_rec ? arg_rec->convert : false);
			}

			// fill in missing arguments
			if (num_copied < num_args)
			{
				for (; num_copied < num_args; ++num_copied)
				{
					if (argument_record const & arg{ func.args[num_copied] }; arg.value)
					{
						call.args.push_back(arg.value, arg.convert);
					}
					else
					{
						break;
					}
				}

				VERIFY("NOT ENOUGH ARGUMENTS" && (num_args <= num_copied));
			}

			OBJECT result{ func.impl(call) };

			// TODO: validate result

			return result;
		}
	};
}

// cppfunction delete
namespace ism { template <> struct DefaultDelete<api::CppFunctionObject> : DefaultDelete<api::BaseObject> {}; }

// cppfunction handle
namespace ism::api
{
	template <> class Handle<CppFunctionObject> : public BaseHandle<CppFunctionObject>
	{
		ISM_HANDLE(CppFunctionObject);

	public:
		Handle() = default;

		~Handle() = default;

		NODISCARD OBJECT name() const { return attr("__name__"); }
	};
}

namespace ism::api
{
	NODISCARD inline OBJECT Handle<FunctionObject>::cpp_function() const noexcept
	{
		return isinstance<CPP_FUNCTION>(*this) ? CPP_FUNCTION(*this) : nullptr;
	}
}

#endif // !_ISM_CPPFUNCTION_OBJECT_HPP_
