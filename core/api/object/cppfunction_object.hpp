#ifndef _ISM_CPPFUNCTION_OBJECT_HPP_
#define _ISM_CPPFUNCTION_OBJECT_HPP_

#include <core/api/initimpl.hpp>

// cppfunction
namespace ism
{
	// cppfunction object
	class ISM_API CppFunctionObject : public FunctionObject
	{
		ISM_OBJECT_TYPED(CppFunctionObject, FunctionObject);

	protected:
		static void _bind_class(OBJECT scope);

	public:
		function_record m_record{};

		NODISCARD auto & operator*() const { return const_cast<function_record &>(m_record); }

		NODISCARD auto * operator->() const { return const_cast<function_record *>(&m_record); }

		virtual ~CppFunctionObject() override {}

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

			if (sizeof(Capture) <= sizeof(m_record.data))
			{
				::new((Capture *)&m_record.data) Capture{ FWD(func) };

				if constexpr (!std::is_trivially_destructible_v<Func>)
				{
					m_record.free_data = [](function_record * f) { ((Capture *)&f->data)->~Capture(); };
				}
			}
			else
			{
				m_record.data[0] = memnew(Capture{ FWD(func) });

				m_record.free_data = [](function_record * f) { memdelete((Capture *)f->data[0]); };
			}

			// convert function arguments and perform the actual function call
			m_record.impl = [](function_call & call) -> OBJECT
			{
				argument_loader<Args...> args{};

				if (!args.load_args(call.args)) { call.try_next_overload = true; return nullptr; }

				attr::process_attributes<Extra...>::precall(call);

				auto data{ (sizeof(Capture) <= sizeof(call.record.data) ? &call.record.data : call.record.data[0]) };

				auto capture{ const_cast<Capture *>(reinterpret_cast<Capture const *>(data)) };

				ReturnPolicy policy{ return_policy_override<Return>::policy(call.record.policy) };

				using Guard = attr::extract_guard_t<Extra...>;

				using Yield = make_caster<std::conditional_t<std::is_void_v<Return>, void_type, Return>>;

				OBJECT result{ Yield::cast(std::move(args).call<Return, Guard>(capture->value), policy, call.parent) };

				attr::process_attributes<Extra...>::postcall(call, result);

				return result;
			};

			// process function attributes
			attr::process_attributes<Extra...>::init(m_record, FWD(extra)...);

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
				m_record.is_stateless = true;
				m_record.data[1] = const_cast<void *>(reinterpret_cast<void const *>(&typeid(Return(*)(Args...))));
			}
		}

		void initialize_generic(std::type_info const * const * info_in, size_t argc_in);

		static OBJECT dispatcher(OBJECT callable, OBJECT const * argv, size_t argc);
	};

	// cppfunction delete
	template <> struct DefaultDelete<CppFunctionObject> : DefaultDelete<BaseObject> {};

	// cppfunction check
#define ISM_CPPFUNCTION_CHECK(o) (isinstance<CPP_FUNCTION>(o))

	// cppfunction handle
	template <> class Handle<CppFunctionObject> : public BaseHandle<CppFunctionObject>
	{
		ISM_HANDLE_DEFAULT(CppFunctionObject, ISM_CPPFUNCTION_CHECK);

	public:
		NODISCARD auto name() const { return attr("__name__"); }

		NODISCARD auto signature() const { return attr("__text_signature__"); }
	};
}

// functions
namespace ism
{
	NODISCARD inline OBJECT Handle<FunctionObject>::cpp_function() const
	{
		return CPP_FUNCTION::check_(*this) ? CPP_FUNCTION(*this) : nullptr;
	}
}

#endif // !_ISM_CPPFUNCTION_OBJECT_HPP_
