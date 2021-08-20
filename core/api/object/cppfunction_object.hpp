#ifndef _ISM_CPPFUNCTION_OBJECT_HPP_
#define _ISM_CPPFUNCTION_OBJECT_HPP_

#include <core/api/initimpl.hpp>

// cppfunction
namespace ism
{
	// cppfunction object
	class ISM_API CppFunctionObject : public FunctionObject
	{
		ISM_OBJECT(CppFunctionObject, FunctionObject);

	protected:
		static void _bind_methods();

	public:
		FunctionRecord * m_record{};

		NODISCARD auto & operator*() const { return const_cast<FunctionRecord &>(*m_record); }

		NODISCARD auto * operator->() const { return const_cast<FunctionRecord *>(m_record); }

		virtual ~CppFunctionObject() override;

		CppFunctionObject() noexcept;
		
		CppFunctionObject(CppFunctionObject const & value) : CppFunctionObject{} { m_record = value.m_record; }
		
		CppFunctionObject(CppFunctionObject && value) noexcept : CppFunctionObject{} { swap(std::move(value)); }
		
		CppFunctionObject & operator=(CppFunctionObject const & value) { CppFunctionObject temp{ value }; return swap(temp); }
		
		CppFunctionObject & operator=(CppFunctionObject && value) noexcept { return swap(std::move(value)); }
		
		CppFunctionObject & swap(CppFunctionObject & other) noexcept { if (this != std::addressof(other)) { std::swap(m_record, other.m_record); } return (*this); }

		template <class Return, class ... Args, class ... Extra
		> CppFunctionObject(Return(*f)(Args...), Extra && ... extra) : CppFunctionObject{}
		{
			initialize(f, f, FWD(extra)...);
		}

		template <class Func, class ... Extra, class = std::enable_if_t<mpl::is_lambda_v<Func>>
		> CppFunctionObject(Func && f, Extra && ... extra) : CppFunctionObject{}
		{
			initialize(FWD(f), (mpl::function_signature_t<Func> *)0, FWD(extra)...);
		}

		template <class Return, class Class, class ... Args, class ... Extra
		> CppFunctionObject(Return(Class::*f)(Args...), Extra && ... extra) : CppFunctionObject{}
		{
			initialize([f](Class * c, Args ... args) -> Return { return (c->*f)(args...); }, (Return(*)(Class *, Args...))0, FWD(extra)...);
		}

		template <class Return, class Class, class ... Args, class ... Extra
		> CppFunctionObject(Return(Class::*f)(Args...) &, Extra && ... extra) : CppFunctionObject{}
		{
			initialize([f](Class * c, Args ... args) -> Return { return (c->*f)(args...); }, (Return(*)(Class *, Args...))0, FWD(extra)...);
		}

		template <class Return, class Class, class ... Args, class ... Extra
		> CppFunctionObject(Return(Class::*f)(Args...) const, Extra && ... extra) : CppFunctionObject{}
		{
			initialize([f](Class const * c, Args ... args) -> Return { return (c->*f)(args...); }, (Return(*)(Class const *, Args...))0, FWD(extra)...);
		}

		template <class Return, class Class, class ... Args, class ... Extra
		> CppFunctionObject(Return(Class::*f)(Args...) const &, Extra && ... extra) : CppFunctionObject{}
		{
			initialize([f](Class const * c, Args ... args) -> Return { return (c->*f)(args...); }, (Return(*)(Class const *, Args...))0, FWD(extra)...);
		}

	protected:
		template <class Func, class Return, class ... Args, class ... Extra
		> void initialize(Func && func, Return(*)(Args...), Extra && ... extra);

		void initialize_generic(FunctionRecord * record_in, std::type_info const * const * info_in, size_t argc_in);
	};

	// cppfunction delete
	template <> struct DefaultDelete<CppFunctionObject> : DefaultDelete<Object> {};

	// cppfunction check
#define ISM_CHECK_CPPFUNCTION(o) (ism::isinstance<ism::CPP_FUNCTION>(o))

	// cppfunction handle
	template <> class Handle<CppFunctionObject> : public Ref<CppFunctionObject>
	{
		ISM_HANDLE(CppFunctionObject, ISM_CHECK_CPPFUNCTION);

	public:
		NODISCARD auto name() const { return attr("__name__"); }

		NODISCARD auto signature() const { return attr("__text_signature__"); }
	};
}

// functions
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_FUNC(OBJ) cppfunction_vectorcall(OBJ callable, OBJ const * argv, size_t argc);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD inline OBJ Handle<FunctionObject>::cpp_function() const
	{
		return CPP_FUNCTION::check_(*this) ? CPP_FUNCTION(*this) : nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Func, class Return, class ... Args, class ... Extra
	> inline void CppFunctionObject::initialize(Func && func, Return(*)(Args...), Extra && ... extra)
	{
		FunctionRecord * rec{ memnew(FunctionRecord{}) };

		struct Capture { std::remove_reference_t<Func> value; };

		if (sizeof(Capture) <= sizeof(rec->data))
		{
			::new((Capture *)&rec->data) Capture{ FWD(func) };

			if constexpr (!std::is_trivially_destructible_v<Func>)
			{
				rec->free_data = [](FunctionRecord * f) { ((Capture *)&f->data)->~Capture(); };
			}
		}
		else
		{
			rec->data[0] = memnew(Capture{ FWD(func) });

			rec->free_data = [](FunctionRecord * f) { memdelete((Capture *)f->data[0]); };
		}

		// convert function arguments and perform the actual function call
		rec->impl = [](FunctionCall & call) -> OBJ
		{
			ArgumentLoader<Args...> args{};

			if (!args.load_args(call.args)) { call.try_next_overload = true; return nullptr; }

			attr::process_attributes<Extra...>::precall(call);

			auto data{ (sizeof(Capture) <= sizeof(call.record.data) ? &call.record.data : call.record.data[0]) };

			auto capture{ const_cast<Capture *>(reinterpret_cast<Capture const *>(data)) };

			ReturnPolicy policy{ return_policy_override<Return>::policy(call.record.policy) };

			using Guard = attr::extract_guard_t<Extra...>;

			using Yield = make_caster<std::conditional_t<std::is_void_v<Return>, void_type, Return>>;

			OBJ result{ Yield::cast(std::move(args).call<Return, Guard>(capture->value), policy, call.parent) };

			attr::process_attributes<Extra...>::postcall(call, result);

			return result;
		};

		// process function attributes
		attr::process_attributes<Extra...>::init(*rec, FWD(extra)...);

		// collect type info
		constexpr size_t argc{ sizeof...(Args) };
		Array<std::type_info const *, argc> types{};
		mpl::for_types_i<Args...>([&](size_t i, auto tag) { types[i] = &typeid(TAG_TYPE(tag)); });

		// initialize generic
		initialize_generic(rec, types, argc);

		// stash some additional information used in 'functional.h'
		if constexpr (std::is_convertible_v<Func, Return(*)(Args...)> && sizeof(Capture) == sizeof(void *))
		{
			rec->is_stateless = true;
			rec->data[1] = const_cast<void *>(reinterpret_cast<void const *>(&typeid(Return(*)(Args...))));
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_CPPFUNCTION_OBJECT_HPP_
