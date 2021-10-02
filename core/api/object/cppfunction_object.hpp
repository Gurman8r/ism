#ifndef _ISM_CPPFUNCTION_OBJECT_HPP_
#define _ISM_CPPFUNCTION_OBJECT_HPP_

#include <core/api/init.hpp>

// cppfunction
namespace ism
{
	// cppfunction object
	class ISM_API CppFunctionObject : public FunctionObject
	{
		OBJECT_COMMON(CppFunctionObject, FunctionObject);

		friend class CPP_FUNCTION;

	public:
		ism::FunctionRecord * m_record{};

		NODISCARD auto & operator*() const { return const_cast<ism::FunctionRecord &>(*m_record); }

		NODISCARD auto * operator->() const { return const_cast<ism::FunctionRecord *>(m_record); }

		virtual ~CppFunctionObject() override;

		CppFunctionObject() noexcept : FunctionObject{ &cppfunction_vectorcall } {}

		NON_COPYABLE(CppFunctionObject);
		
		CppFunctionObject(CppFunctionObject && value) noexcept : CppFunctionObject{} { swap(std::move(value)); }
		
		CppFunctionObject & operator=(CppFunctionObject && value) noexcept { return swap(std::move(value)); }
		
		CppFunctionObject & swap(CppFunctionObject & other) noexcept { if (this != std::addressof(other)) { std::swap(m_record, other.m_record); } return (*this); }

		template <class Return, class ... Args, class ... Extra
		> CppFunctionObject(Return(*f)(Args...), Extra && ... extra) : CppFunctionObject{}
		{
			this->initialize(f, f, FWD(extra)...);
		}

		template <class Func, class ... Extra, class = std::enable_if_t<mpl::is_lambda_v<Func>>
		> CppFunctionObject(Func && f, Extra && ... extra) : CppFunctionObject{}
		{
			this->initialize(FWD(f), (mpl::function_signature_t<Func> *)0, FWD(extra)...);
		}

		template <class Return, class Class, class ... Args, class ... Extra
		> CppFunctionObject(Return(Class::*f)(Args...), Extra && ... extra) : CppFunctionObject{}
		{
			this->initialize([f](Class * c, Args ... args) -> Return { return (c->*f)(args...); }, (Return(*)(Class *, Args...))0, FWD(extra)...);
		}

		template <class Return, class Class, class ... Args, class ... Extra
		> CppFunctionObject(Return(Class::*f)(Args...) &, Extra && ... extra) : CppFunctionObject{}
		{
			this->initialize([f](Class * c, Args ... args) -> Return { return (c->*f)(args...); }, (Return(*)(Class *, Args...))0, FWD(extra)...);
		}

		template <class Return, class Class, class ... Args, class ... Extra
		> CppFunctionObject(Return(Class::*f)(Args...) const, Extra && ... extra) : CppFunctionObject{}
		{
			this->initialize([f](Class const * c, Args ... args) -> Return { return (c->*f)(args...); }, (Return(*)(Class const *, Args...))0, FWD(extra)...);
		}

		template <class Return, class Class, class ... Args, class ... Extra
		> CppFunctionObject(Return(Class::*f)(Args...) const &, Extra && ... extra) : CppFunctionObject{}
		{
			this->initialize([f](Class const * c, Args ... args) -> Return { return (c->*f)(args...); }, (Return(*)(Class const *, Args...))0, FWD(extra)...);
		}

	protected:
		void initialize_generic(ism::FunctionRecord * record_in, std::type_info const * const * info_in, size_t argc_in, bool prepend = false);

		template <class Func, class Return, class ... Args, class ... Extra
		> void initialize(Func && func, Return(*)(Args...), Extra && ... extra)
		{
			ism::FunctionRecord * rec{ memnew(ism::FunctionRecord{}) };

			struct Capture { std::remove_reference_t<Func> value; };

			if (sizeof(Capture) <= sizeof(rec->data))
			{
				::new((Capture *)&rec->data) Capture{ FWD(func) };

				if constexpr (!std::is_trivially_destructible_v<Func>)
				{
					rec->free_data = [](ism::FunctionRecord * f) { ((Capture *)&f->data)->~Capture(); };
				}
			}
			else
			{
				rec->data[0] = memnew(Capture{ FWD(func) });

				rec->free_data = [](ism::FunctionRecord * f) { memdelete((Capture *)f->data[0]); };
			}

			// convert function arguments and perform the actual function call
			rec->impl = [](ism::FunctionCall & call) -> OBJ
			{
				ism::ArgumentLoader<Args...> args{};

				if (!args.load_args(call.args)) { call.try_next_overload = true; return nullptr; }

				attr::process_attributes<Extra...>::precall(call);

				auto data{ (sizeof(Capture) <= sizeof(call.record.data) ? &call.record.data : call.record.data[0]) };

				auto capture{ const_cast<Capture *>(reinterpret_cast<Capture const *>(data)) };

				ReturnValuePolicy_ policy{ ism::return_policy_override<Return>::policy(call.record.policy) };

				using Guard = attr::extract_guard_t<Extra...>;

				using Yield = ism::make_caster<std::conditional_t<std::is_void_v<Return>, void_type, Return>>;

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

			// prepend?
			constexpr bool prepend{ mpl::contains_v<attr::prepend, mpl::type_list<Extra...>> };

			// initialize generic
			initialize_generic(rec, types, argc, prepend);

			// stash some additional information used in 'functional.h'
			if constexpr (std::is_convertible_v<Func, Return(*)(Args...)> && sizeof(Capture) == sizeof(void *))
			{
				rec->is_stateless = true;
				rec->data[1] = const_cast<void *>(reinterpret_cast<void const *>(&typeid(Return(*)(Args...))));
			}
		}

	public:
		static OBJ cppfunction_vectorcall(OBJ callable, OBJ const * argv, size_t argc);
	};

	// cppfunction delete
	template <> struct DefaultDelete<CppFunctionObject> : DefaultDelete<Object> {};

	// cppfunction check
#define OBJECT_CHECK_CPPFUNCTION(o) (ism::isinstance<ism::CPP_FUNCTION>(o))

	// cppfunction ref
	class CPP_FUNCTION : public Ref<CppFunctionObject>
	{
		REF_COMMON(CPP_FUNCTION, OBJECT_CHECK_CPPFUNCTION);

	public:
		NODISCARD auto name() const { return attr("__name__"); }

		NODISCARD auto signature() const { return attr("__text_signature__"); }
	};
}

NODISCARD inline ism::OBJ ism::FUNCTION::cpp_function() const
{
	return ism::CPP_FUNCTION::check_(*this) ? ism::CPP_FUNCTION(*this) : nullptr;
}


#endif // !_ISM_CPPFUNCTION_OBJECT_HPP_
