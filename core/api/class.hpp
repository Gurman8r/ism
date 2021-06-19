#ifndef _ISM_CLASS_HPP_
#define _ISM_CLASS_HPP_

#include <core/api/object/generic_object.hpp>

namespace ism::api
{
	// class interface
	template <class type_, class ... options_
	> class CLASS_ : public TYPE
	{
	public:
		using value_type = typename type_;
	
		template <class ... Extra
		> CLASS_(OBJECT scope, cstring name, Extra && ... extra) : TYPE{ TYPE::check_(scope) ? scope : memnew(TypeObject) }
		{
			attr::process_attributes<Extra...>::init(*m_ref, FWD(extra)...);
		}

		template <class ... Args, class ... Extra
		> CLASS_ & def(initimpl::constructor<Args...> && init, Extra && ... extra)
		{
			return FWD(init).execute(*this, FWD(extra)...);
		}

		template <class ... Args, class ... Extra
		> CLASS_ & def(initimpl::factory<Args...> && init, Extra && ... extra)
		{
			return FWD(init).execute(*this, FWD(extra)...);
		}
	
		template <class Func, class ... Extra
		> CLASS_ & def(cstring name, Func && func, Extra && ... extra)
		{
			CPP_FUNCTION cf({
				method_adaptor<value_type>(FWD(func)),
				attr::name(name),
				attr::is_method(*this),
				attr::sibling(attr(name))
				FWD(extra)... });
			attr(name) = cf;
			return (*this);
		}
	
		template <class Func, class ... Extra
		> CLASS_ & def_static(cstring name, Func && func, Extra && ... extra)
		{
			CPP_FUNCTION cf({
				FWD(func),
				attr::name(name),
				attr::scope(*this),
				attr::sibling(attr(name)),
				FWD(extra)... });
			attr(name) = cf;
			return (*this);
		}

		template <class C, class D, class ... Extra
		> CLASS_ & def_readwrite(cstring name, D C:: * pm, Extra &&... extra)
		{
			static_assert(std::is_same_v<C, value_type> || std::is_base_of_v<C, value_type>, "def_readwrite() requires a class member (or base class member)");
			CPP_FUNCTION fget({ [pm](value_type const & c) -> D const & { return c.*pm; }, attr::is_method(*this) });
			CPP_FUNCTION fset({ [pm](value_type & c, D const & value) { c.*pm = value; }, attr::is_method(*this) });
			return def_property(name, fget, fset, ReturnPolicy_ReferenceInternal, FWD(extra)...);
		}

		template <class C, class D, class ... Extra
		> CLASS_ & def_readonly(cstring name, const D C:: * pm, Extra && ... extra)
		{
			static_assert(std::is_same_v<C, value_type> || std::is_base_of_v<C, value_type>, "def_readonly() requires a class member (or base class member)");
			CPP_FUNCTION fget({ [pm](value_type const & c) -> D const & { return c.*pm; }, attr::is_method(*this) });
			return def_property_readonly(name, fget, ReturnPolicy_ReferenceInternal, FWD(extra)...);
		}

		template <class D, class ... Extra
		> CLASS_ & def_readwrite_static(cstring name, D * pm, Extra && ... extra)
		{
			CPP_FUNCTION fget({ [pm](OBJECT) -> D const & { return *pm; }, attr::scope(*this) });
			CPP_FUNCTION fset({ [pm](OBJECT, D const & value) { *pm = value; }, attr::scope(*this) });
			return def_property_static(name, fget, fset, ReturnPolicy_Reference, FWD(extra)...);
		}

		template <class D, class ... Extra
		> CLASS_ & def_readonly_static(cstring name, const D * pm, Extra && ... extra)
		{
			CPP_FUNCTION fget({ [pm](OBJECT) -> D const & { return *pm; }, attr::scope(*this) });
			return def_property_readonly_static(name, fget, ReturnPolicy_Reference, FWD(extra)...);
		}

		template <class Getter, class ... Extra
		> CLASS_ & def_property_readonly(cstring name, Getter const & fget, Extra && ... extra)
		{
			return def_property_readonly(name, CPP_FUNCTION({ method_adaptor<value_type>(fget) }), ReturnPolicy_ReferenceInternal, FWD(extra)...);
		}

		template <class ... Extra
		> CLASS_ & def_property_readonly(cstring name, CPP_FUNCTION const & fget, Extra && ... extra)
		{
			return def_property(name, fget, nullptr, FWD(extra)...);
		}

		template <class Getter, class ... Extra
		> CLASS_ & def_property_readonly_static(cstring name, Getter const & fget, Extra && ... extra)
		{
			return def_property_readonly_static(name, CPP_FUNCTION({ fget }), ReturnPolicy_Reference, FWD(extra)...);
		}

		template <class ... Extra
		> CLASS_ & def_property_readonly_static(cstring name, CPP_FUNCTION const & fget, Extra && ... extra)
		{
			return def_property_static(name, fget, nullptr, FWD(extra)...);
		}

		template <class Getter, class Setter, class ... Extra
		> CLASS_ & def_property(cstring name, Getter const & fget, Setter const & fset, Extra && ... extra)
		{
			return def_property(name, fget, CPP_FUNCTION({ method_adaptor<value_type>(fset) }), FWD(extra)...);
		}

		template <class Getter, class ... Extra
		> CLASS_ & def_property(cstring name, Getter const & fget, CPP_FUNCTION const & fset, Extra && ... extra)
		{
			return def_property(name, CPP_FUNCTION({ method_adaptor<value_type>(fget) }), fset, ReturnPolicy_ReferenceInternal, FWD(extra)...);
		}

		template <class ... Extra
		> CLASS_ & def_property(cstring name, CPP_FUNCTION const & fget, CPP_FUNCTION const & fset, Extra && ... extra)
		{
			return def_property_static(name, fget, fset, attr::is_method(*this), FWD(extra)...);
		}

		template <class Getter, class ... Extra
		> CLASS_ & def_property_static(cstring name, Getter const & fget, CPP_FUNCTION const & fset, Extra && ... extra)
		{
			return def_property_static(name, CPP_FUNCTION({ fget }), fset, ReturnPolicy_Reference, FWD(extra)...);
		}
	
		template <class ... Extra
		> CLASS_ & def_property_static(cstring name, CPP_FUNCTION const & fget, CPP_FUNCTION const & fset, Extra && ... extra)
		{
			function_record * rec_fget{ fget ? &fget->m_func : nullptr }, * rec_fset{ fset ? &fset->m_func : nullptr };

			if (rec_fget) { attr::process_attributes<Extra...>::init(*rec_fget, FWD(extra)...); }
	
			if (rec_fset) { attr::process_attributes<Extra...>::init(*rec_fset, FWD(extra)...); }
	
			m_ref->tp_dict[name] = PROPERTY({ fget, fset });
			//type_setattr(*this, STR(name), PROPERTY({ fget, fset }));
			//attr(name) = PROPERTY({ fget, fset });
			
			return (*this);
		}
	};
}

#endif // !_ISM_CLASS_HPP_
