#ifndef _ISM_MODSUPPORT_HPP_
#define _ISM_MODSUPPORT_HPP_

#include <core/api/detail/init.hpp>

// data
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_DATA(CoreType)		_CoreCppFunction_Type;
#define CoreCppFunction_Type	(ism::TYPE(&ism::_CoreCppFunction_Type))

	ISM_API_DATA(CoreType)		_CoreModule_Type;
#define CoreModule_Type			(ism::TYPE(&ism::_CoreModule_Type))

	ISM_API_DATA(CoreType)		_CoreGeneric_Type;
#define CoreGeneric_Type		(ism::TYPE(&ism::_CoreGeneric_Type))

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// cpp_function
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class NODISCARD ISM_API CoreCppFunction : public CoreFunction
	{
		ISM_CLASS_DEFAULT(CoreCppFunction, CoreFunction, CoreCppFunction_Type);
		
		detail::function_record m_data{};

	public:
		template <class Return, class ... Args, class ... Extra
		> CoreCppFunction(Return(*f)(Args...), Extra && ... extra) : self_type{ detail::function_record{
			f,
			f,
			FWD(extra)...
		} } {}

		template <class Func, class ... Extra, class = std::enable_if_t<is_lambda_v<Func>>
		> CoreCppFunction(Func && f, Extra && ... extra) : self_type{ detail::function_record{
			FWD(f),
			(function_signature_t<Func> *)0,
			FWD(extra)...
		} } {}
		
		template <class Return, class Class, class ... Args, class ... Extra
		> CoreCppFunction(Return(Class::*f)(Args...), Extra && ... extra) : self_type{ detail::function_record{
			[f](Class * c, Args ... args) -> Return { return (c->*f)(args...); },
			(Return(*)(Class *, Args...))0,
			FWD(extra)...
		} } {}

		template <class Return, class Class, class ... Args, class ... Extra
		> CoreCppFunction(Return(Class::*f)(Args...) &, Extra && ... extra) : self_type{ detail::function_record{
			[f](Class * c, Args ... args) -> Return { return (c->*f)(args...); },
			(Return(*)(Class *, Args...))0,
			FWD(extra)...
		} } {}

		template <class Return, class Class, class ... Args, class ... Extra
		> CoreCppFunction(Return(Class::*f)(Args...) const, Extra && ... extra) : self_type{ detail::function_record{
			[f](Class const * c, Args ... args) -> Return { return (c->*f)(args...); },
			(Return(*)(Class const *, Args...))0,
			FWD(extra)...
		} } {}

		template <class Return, class Class, class ... Args, class ... Extra
		> CoreCppFunction(Return(Class::*f)(Args...) const &, Extra && ... extra) : self_type{ detail::function_record{
			[f](Class const * c, Args ... args) -> Return { return (c->*f)(args...); },
			(Return(*)(Class const *, Args...))0,
			FWD(extra)...
		} } {}

		NODISCARD operator detail::function_record * () const { return const_cast<detail::function_record *>(&m_data); }

		NODISCARD auto name() const { return attr("__name__"); }

	protected:
		explicit CoreCppFunction(detail::function_record && value) noexcept
			: base_type{ type_static(), (vectorcallfunc)dispatcher }, m_data{ std::move(value) }
		{
		}

		static OBJECT dispatcher(OBJECT callable, OBJECT const * argv, size_t argc)
		{
			detail::function_record const & func{ ***CPP_FUNCTION(callable) };

			detail::function_call call{ func, (0 < argc ? argv[0] : nullptr) };

			return func.impl(call(argv, argc));
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// module
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class NODISCARD ISM_API CoreModule : public CoreObject
	{
		ISM_CLASS_DEFAULT(CoreModule, CoreObject, CoreModule_Type);

	public:
		DICT		m_dict{ CoreDict{} };
		STR			m_name{};
		STR			m_doc{};
		inquiry		m_clear{};
		freefunc	m_free{};

		CoreModule(cstring name) : self_type{}
		{
			m_name = STR(name);
		}

		template <class Func, class ... Extra
		> CoreModule & def(cstring name, Func && func, Extra && ... extra)
		{
			CPP_FUNCTION cf({
				FWD(func),
				detail::name(name),
				detail::scope(this),
				detail::sibling(attr(name)),
				FWD(extra)... });
			attr(cf->name()) = cf;
			return (*this);
		}

		MODULE def_submodule(cstring name, cstring doc = "")
		{
			return MODULE{};
		}

		template <class Name = cstring, class O = OBJECT
		> void add_object(Name && name, O && value, bool overwrite = false)
		{
			auto i{ detail::object_or_cast(FWD(name)) };
			if (m_dict->contains(i) && !overwrite) { return; }
			m_dict[i] = detail::object_or_cast(FWD(value));
		}

		void reload()
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline MODULE create_extension_module(cstring name)
	{
		DICT d{ get_default_interpreter()->modules };
		auto i{ detail::object_or_cast(name) };
		if (d->contains(i)) { return nullptr; }
		else { return d[i] = MODULE({ name }); }
	}

	inline MODULE import_module(cstring name)
	{
		DICT d{ get_default_interpreter()->modules };
		auto i{ detail::object_or_cast(name) };
		if (!d->contains(i)) { return nullptr; }
		else { return d[i]; }
	}

	inline DICT globals()
	{
		if (auto frame{ get_default_frame() }; frame)
		{
			return CHECK(frame->globals);
		}
		else
		{
			return import_module("__main__").attr("__dict__");
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// generic
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API CoreGeneric : public CoreObject
	{
		ISM_CLASS_DEFAULT(CoreGeneric, CoreObject, CoreGeneric_Type);

	public:
		DICT m_dict{ CoreDict{} };
		vectorcallfunc m_vectorcall{};

	protected:
		void def_property_static_impl(cstring name, OBJECT fget, OBJECT fset, detail::function_record * rec_func)
		{
			//bool const is_static{ rec_func && !(rec_func->is_method && rec_func->scope) };
			//bool const has_doc{ rec_func && !rec_func->doc.empty() };
			//OBJECT property{ is_static ? typeof<STATIC_PROPERTY>() : typeof<PROPERTY>() };
			attr(name) = PROPERTY({ fget, fset, nullptr });
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// class
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class type_, class ... options_
	> class CoreClass : public CoreGeneric
	{
		template <class T> using is_holder = detail::is_holder_type<type_, T>;
		template <class T> using is_subtype = is_strict_base_of<type_, T>;
		template <class T> using is_base = is_strict_base_of<T, type_>;
		template <class T> struct is_valid_class_option : any_of<is_holder<T>, is_subtype<T>, is_base<T>> {};

		ISM_CLASS_DEFAULT(CoreClass, CoreGeneric, CoreGeneric_Type);

		Scary<type_> m_holder{};

	public:
		using type = typename type_;
		NODISCARD auto & holder() { return m_holder; }
		NODISCARD auto const & holder() const { return m_holder; }
		NODISCARD auto const * value_ptr() const { return m_holder.get(); }

	public:
		template <class ... Extra
		> CoreClass(OBJECT scope, cstring name, Extra && ... extra) : self_type{}
		{
			//ob_type.revalue(COMPOSE(CoreType, t, &)
			//{
			//	t.tp_name = name;
			//	t.tp_basicsize = sizeof(type);
			//	t.tp_flags = TypeFlags_Default | TypeFlags_HeapType;
			//	t.tp_base = CoreGeneric_Type;
			//	t.tp_dict = DICT(CoreDict{});
			//	t.tp_delete = (destructor)[](CoreObject * ptr) { memdelete((CoreClass *)ptr); };
			//});
			//
			//detail::process_attributes<Extra...>::init(**ob_type, FWD(extra)...);
		}

		template <class Func, class ... Extra
		> CoreClass & def(cstring name, Func && func, Extra && ... extra)
		{
			CPP_FUNCTION cf({
				detail::method_adaptor<type>(FWD(func)),
				detail::name(name),
				detail::is_method(this),
				detail::sibling(attr(name)),
				FWD(extra)... });
			attr(cf->name()) = cf;
			return (*this);
		}

		template <class ... Args, class ... Extra
		> CoreClass & def(detail::initimpl::constructor<Args...> && init, Extra && ... extra) noexcept
		{
			return FWD(init).execute(*this, FWD(extra)...);
		}

		template <class ... Args, class ... Extra
		> CoreClass & def(detail::initimpl::factory<Args...> && init, Extra && ... extra) noexcept
		{
			return FWD(init).execute(*this, FWD(extra)...);
		}

		template <class Func, class ... Extra
		> CoreClass & def_static(cstring name, Func && func, Extra && ... extra)
		{
			CPP_FUNCTION cf({
				FWD(func),
				detail::name(name),
				detail::scope(this),
				detail::sibling(attr(name)),
				FWD(extra)... });
			attr(cf->name()) = cf;
			return (*this);
		}

		template <class C, class D, class ... Extra
		> CoreClass & def_readwrite(cstring name, D C::*pm, Extra && ... extra)
		{
			CPP_FUNCTION fget({ [pm](type const & c) -> D const & { return c.*pm; }, detail::is_method(this) });
			CPP_FUNCTION fset({ [pm](type & c, D const & value) { c.*pm = value; }, detail::is_method(this) });
			return this->def_property(name, fget, fset, ReturnPolicy_ReferenceInternal, FWD(extra)...);
		}

		template <class C, class D, class ... Extra
		> CoreClass & def_readonly(cstring name, D const C::*pm, Extra && ... extra)
		{
			CPP_FUNCTION fget({ [pm](type const & c) -> D const & { return c.*pm; }, detail::is_method(this) });
			return this->def_property_readonly(name, fget, ReturnPolicy_ReferenceInternal, FWD(extra)...);
		}

		template <class D, class ... Extra
		> CoreClass & def_readwrite_static(cstring name, D * pm, Extra && ... extra)
		{
			CPP_FUNCTION fget({ [pm](object) -> D const & { return *pm; }, detail::scope(this) });
			CPP_FUNCTION fset({ [pm](object, D const & value) { *pm = value; }, detail::scope(this) });
			return this->def_property_static(name, fget, fset, ReturnPolicy_Reference, FWD(extra)...);
		}

		template <class D, class ... Extra
		> CoreClass & def_readonly_static(cstring name, D const * pm, Extra && ... extra)
		{
			CPP_FUNCTION fget({ [pm](object) -> D const & { return *pm; }, detail::scope(this) });
			return this->def_property_readonly_static(name, fget, ReturnPolicy_Reference, FWD(extra)...);
		}

		template <class Getter, class ... Extra
		> CoreClass & def_property_readonly(cstring name, Getter const & fget, Extra && ... extra)
		{
			return this->def_property(name, CPP_FUNCTION({ fget }), ReturnPolicy_ReferenceInternal, FWD(extra)...);
		}

		template <class ... Extra
		> CoreClass & def_property_readonly(cstring name, CPP_FUNCTION const & fget, Extra && ... extra)
		{
			return this->def_property(name, fget, nullptr, FWD(extra)...);
		}

		template <class Getter, class ... Extra
		> CoreClass & def_property_readonly_static(cstring name, Getter const & fget, Extra && ... extra)
		{
			return this->def_property_readonly_static(name, CPP_FUNCTION({ fget }), ReturnPolicy_Reference, FWD(extra)...);
		}

		template <class ... Extra
		> CoreClass & def_property_readonly_static(cstring name, CPP_FUNCTION const & fget, Extra && ... extra)
		{
			return this->def_property_static(name, fget, nullptr, FWD(extra)...);
		}

		template <class Getter, class Setter, class ... Extra
		> CoreClass & def_property(cstring name, Getter const & fget, Setter const & fset, Extra && ... extra)
		{
			return this->def_property(name, fget, CPP_FUNCTION({ detail::method_adaptor<type>(fset) }), FWD(extra)...);
		}

		template <class Getter, class ... Extra
		> CoreClass & def_property(cstring name, Getter const & fget, CPP_FUNCTION const & fset, Extra && ... extra)
		{
			return this->def_property_static(name, CPP_FUNCTION({ fget }), fset, ReturnPolicy_ReferenceInternal, FWD(extra)...);
		}

		template <class ... Extra
		> CoreClass & def_property(cstring name, CPP_FUNCTION const & fget, CPP_FUNCTION const & fset, Extra && ... extra)
		{
			return this->def_property_static(name, fget, fset, detail::is_method(this), FWD(extra)...);
		}

		template <class Getter, class ... Extra
		> CoreClass & def_property_static(cstring name, Getter const & fget, CPP_FUNCTION const & fset, Extra && ... extra)
		{
			return this->def_property_static(name, CPP_FUNCTION({ fget }), fset, ReturnPolicy_Reference, FWD(extra)...);
		}

		template <class ... Extra
		> CoreClass & def_property_static(cstring name, CPP_FUNCTION const & fget, CPP_FUNCTION const & fset, Extra && ... extra)
		{
			detail::function_record * rec_fget{ &(***fget) }, * rec_fset{ &(***fset) }, * rec_active{ rec_fget };

			if (rec_fget) {
				String & doc_prev{ rec_fget->doc };
				detail::process_attributes<Extra...>::init(*rec_fget, FWD(extra)...);
				if (!rec_fget->doc.empty() && rec_fget->doc != doc_prev) { rec_fget->doc = rec_fget->doc; }
			}

			if (rec_fset) {
				String & doc_prev{ rec_fset->doc };
				detail::process_attributes<Extra...>::init(*rec_fset, FWD(extra)...);
				if (!rec_fset->doc.empty() && rec_fset->doc != doc_prev) { rec_fset->doc = rec_fset->doc; }
				if (!rec_active) { rec_active = rec_fset; }
			}

			def_property_static_impl(name, fget, fset, rec_active);
			return (*this);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ism::detail
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline std::pair<decltype(RuntimeState::_registry::registered_types_core)::iterator, bool> all_type_info_get_cache(TYPE const & t)
	{
		auto res{ get_default_registry().registered_types_core.try_emplace(t) };
		if (res.second)
		{
			// set up a weak reference to automatically remove it if the type gets destroyed
		}
		return res;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_MODSUPPORT_HPP_
