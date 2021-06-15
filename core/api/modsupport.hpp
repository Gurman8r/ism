#ifndef _ISM_MODSUPPORT_HPP_
#define _ISM_MODSUPPORT_HPP_

#include <core/api/object/cppfunction_object.hpp>
#include <core/api/object/module_object.hpp>
#include <core/api/object/generic_object.hpp>

namespace ism::api
{
	// class
	template <class type_, class ... options_
	> class Class_ : public GenericObject
	{
	private:
		template <class T> using is_holder = api::is_holder_type<type_, T>;
		template <class T> using is_subtype = mpl::is_strict_base_of<type_, T>;
		template <class T> using is_base = mpl::is_strict_base_of<T, type_>;
		template <class T> struct is_valid_class_option : mpl::any_of<is_holder<T>, is_subtype<T>, is_base<T>> {};
	
		Scary<type_> m_holder{};
	
	public:
		using value_type = typename type_;
		NODISCARD auto & holder() { return m_holder; }
		NODISCARD auto const & holder() const { return m_holder; }
		NODISCARD auto const * value_ptr() const { return m_holder.get(); }
	
	public:
		template <class ... Extra
		> Class_(OBJECT scope, cstring name, Extra && ... extra) : self_type{}
		{
			TYPE type = scope.attr(name) = COMPOSE(TypeObject, t)
			{
				t.tp_name = ctti::type_v<value_type>;
			};

			attr::process_attributes<Extra...>::init(**type, FWD(extra)...);
		}
	
		template <class Func, class ... Extra
		> Class_ & def(cstring name, Func && func, Extra && ... extra)
		{
			CPP_FUNCTION cf({
				method_adaptor<value_type>(FWD(func)),
				attr::name(name),
				attr::is_method(this),
				attr::sibling(attr(name)),
				FWD(extra)... });
			attr(cf.name()) = cf;
			return (*this);
		}
	
		template <class ... Args, class ... Extra
		> Class_ & def(initimpl::constructor<Args...> && init, Extra && ... extra) noexcept
		{
			return FWD(init).execute(*this, FWD(extra)...);
		}
	
		template <class ... Args, class ... Extra
		> Class_ & def(initimpl::factory<Args...> && init, Extra && ... extra) noexcept
		{
			return FWD(init).execute(*this, FWD(extra)...);
		}
	
		template <class Func, class ... Extra
		> Class_ & def_static(cstring name, Func && func, Extra && ... extra)
		{
			CPP_FUNCTION cf({
				FWD(func),
				attr::name(name),
				attr::scope(this),
				attr::sibling(attr(name)),
				FWD(extra)... });
			attr(cf.name()) = cf;
			return (*this);
		}
	
		template <class C, class D, class ... Extra
		> Class_ & def_readwrite(cstring name, D C::*pm, Extra && ... extra)
		{
			CPP_FUNCTION fget({ [pm](value_type const & c) -> D const & { return c.*pm; }, attr::is_method(this) });
			CPP_FUNCTION fset({ [pm](value_type & c, D const & value) { c.*pm = value; }, attr::is_method(this) });
			return this->def_property(name, fget, fset, ReturnPolicy_ReferenceInternal, FWD(extra)...);
		}
	
		template <class C, class D, class ... Extra
		> Class_ & def_readonly(cstring name, D const C::*pm, Extra && ... extra)
		{
			CPP_FUNCTION fget({ [pm](value_type const & c) -> D const & { return c.*pm; }, attr::is_method(this) });
			return this->def_property_readonly(name, fget, ReturnPolicy_ReferenceInternal, FWD(extra)...);
		}
	
		template <class D, class ... Extra
		> Class_ & def_readwrite_static(cstring name, D * pm, Extra && ... extra)
		{
			CPP_FUNCTION fget({ [pm](OBJECT) -> D const & { return *pm; }, attr::scope(this) });
			CPP_FUNCTION fset({ [pm](OBJECT, D const & value) { *pm = value; }, attr::scope(this) });
			return this->def_property_static(name, fget, fset, ReturnPolicy_Reference, FWD(extra)...);
		}
	
		template <class D, class ... Extra
		> Class_ & def_readonly_static(cstring name, D const * pm, Extra && ... extra)
		{
			CPP_FUNCTION fget({ [pm](OBJECT) -> D const & { return *pm; }, attr::scope(this) });
			return this->def_property_readonly_static(name, fget, ReturnPolicy_Reference, FWD(extra)...);
		}
	
		template <class Getter, class ... Extra
		> Class_ & def_property_readonly(cstring name, Getter const & fget, Extra && ... extra)
		{
			return this->def_property(name, CPP_FUNCTION({ fget }), ReturnPolicy_ReferenceInternal, FWD(extra)...);
		}
	
		template <class ... Extra
		> Class_ & def_property_readonly(cstring name, CPP_FUNCTION const & fget, Extra && ... extra)
		{
			return this->def_property(name, fget, nullptr, FWD(extra)...);
		}
	
		template <class Getter, class ... Extra
		> Class_ & def_property_readonly_static(cstring name, Getter const & fget, Extra && ... extra)
		{
			return this->def_property_readonly_static(name, CPP_FUNCTION({ fget }), ReturnPolicy_Reference, FWD(extra)...);
		}
	
		template <class ... Extra
		> Class_ & def_property_readonly_static(cstring name, CPP_FUNCTION const & fget, Extra && ... extra)
		{
			return this->def_property_static(name, fget, nullptr, FWD(extra)...);
		}
	
		template <class Getter, class Setter, class ... Extra
		> Class_ & def_property(cstring name, Getter const & fget, Setter const & fset, Extra && ... extra)
		{
			return this->def_property(name, fget, CPP_FUNCTION({ method_adaptor<value_type>(fset) }), FWD(extra)...);
		}
	
		template <class Getter, class ... Extra
		> Class_ & def_property(cstring name, Getter const & fget, CPP_FUNCTION const & fset, Extra && ... extra)
		{
			return this->def_property_static(name, CPP_FUNCTION({ fget }), fset, ReturnPolicy_ReferenceInternal, FWD(extra)...);
		}
	
		template <class ... Extra
		> Class_ & def_property(cstring name, CPP_FUNCTION const & fget, CPP_FUNCTION const & fset, Extra && ... extra)
		{
			return this->def_property_static(name, fget, fset, attr::is_method(this), FWD(extra)...);
		}
	
		template <class Getter, class ... Extra
		> Class_ & def_property_static(cstring name, Getter const & fget, CPP_FUNCTION const & fset, Extra && ... extra)
		{
			return this->def_property_static(name, CPP_FUNCTION({ fget }), fset, ReturnPolicy_Reference, FWD(extra)...);
		}
	
		template <class ... Extra
		> Class_ & def_property_static(cstring name, CPP_FUNCTION const & fget, CPP_FUNCTION const & fset, Extra && ... extra)
		{
			function_record * rec_fget{ &(***fget) }, * rec_fset{ &(***fset) }, * rec_active{ rec_fget };
	
			if (rec_fget)
			{
				String & doc_prev{ rec_fget->doc };
				
				attr::process_attributes<Extra...>::init(*rec_fget, FWD(extra)...);
				
				if (!rec_fget->doc.empty() && rec_fget->doc != doc_prev) { rec_fget->doc = rec_fget->doc; }
			}
	
			if (rec_fset)
			{
				String & doc_prev{ rec_fset->doc };
				
				attr::process_attributes<Extra...>::init(*rec_fset, FWD(extra)...);
				
				if (!rec_fset->doc.empty() && rec_fset->doc != doc_prev) { rec_fset->doc = rec_fset->doc; }
				
				if (!rec_active) { rec_active = rec_fset; }
			}
	
			def_property_static_impl(name, fget, fset, rec_active);

			return (*this);
		}
	};
}

#endif // !_ISM_MODSUPPORT_HPP_
