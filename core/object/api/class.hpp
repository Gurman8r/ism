#ifndef _ISM_CLASS_HPP_
#define _ISM_CLASS_HPP_

#include <core/object/module_object.hpp>

// class
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// class interface
	template <class _type, class ... _options
	> class CLASS_ : public TYPE
	{
	public:
		using holder_type = std::conditional_t<is_base_object_v<_type>, Ref<_type>, _type>;

		using value_type = std::conditional_t<is_base_object_v<_type>, _type, typename holder_type::value_type>;
		
		using type = _type;

	public:
		CLASS_(TYPE target) : TYPE{ VALIDATE(target) } {}

	public:
		template <class ... Args, class ... Extra
		> CLASS_ & def(api::Constructor<Args...> && init, Extra && ... extra)
		{
			return FWD(init).execute(*this, FWD(extra)...); // init.hpp
		}

		template <class ... Args, class ... Extra
		> CLASS_ & def(api::Factory<Args...> && init, Extra && ... extra)
		{
			return FWD(init).execute(*this, FWD(extra)...); // init.hpp
		}

		template <class Func, class ... Extra
		> CLASS_ & def(cstring name, Func && func, Extra && ... extra)
		{
			CPP_FUNCTION cf({
				api::method_adaptor<type>(FWD(func)),
				api::name(name),
				api::is_method(*this),
				api::sibling(api::getattr(*this, name, nullptr)),
				FWD(extra)... });
			return add_object(cf.name(), cf), (*this);
		}

		template <class Func, class ... Extra
		> CLASS_ & def_static(cstring name, Func && func, Extra && ... extra)
		{
			CPP_FUNCTION cf({
				FWD(func),
				api::name(name),
				api::scope(*this),
				api::sibling(api::getattr(*this, name, nullptr)),
				FWD(extra)... });
			return add_object(cf.name(), cf), (*this);
		}

		template <class C, class D, class ... Extra
		> CLASS_ & def_readwrite(cstring name, D C::*pm, Extra && ... extra)
		{
			static_assert(std::is_same_v<C, type> || std::is_base_of_v<C, type>, "def_readwrite() requires a class member (or base class member)");
			CPP_FUNCTION fget({ [pm](type const & c) -> D const & { return c.*pm; }, api::is_method(*this) });
			CPP_FUNCTION fset({ [pm](type & c, D const & value) { c.*pm = value; }, api::is_method(*this) });
			return def_property(name, fget, fset, ReturnPolicy_ReferenceInternal, FWD(extra)...);
		}

		template <class C, class D, class ... Extra
		> CLASS_ & def_readonly(cstring name, D const C::*pm, Extra && ... extra)
		{
			static_assert(std::is_same_v<C, type> || std::is_base_of_v<C, type>, "def_readonly() requires a class member (or base class member)");
			CPP_FUNCTION fget({ [pm](type const & c) -> D const & { return c.*pm; }, api::is_method(*this) });
			return def_property_readonly(name, fget, ReturnPolicy_ReferenceInternal, FWD(extra)...);
		}

		template <class D, class ... Extra
		> CLASS_ & def_readwrite_static(cstring name, D * pm, Extra && ... extra)
		{
			CPP_FUNCTION fget({ [pm](OBJ) -> D const & { return *pm; }, api::scope(*this) });
			CPP_FUNCTION fset({ [pm](OBJ, D const & value) { *pm = value; }, api::scope(*this) });
			return def_property_static(name, fget, fset, ReturnPolicy_Reference, FWD(extra)...);
		}

		template <class D, class ... Extra
		> CLASS_ & def_readonly_static(cstring name, D const * pm, Extra && ... extra)
		{
			CPP_FUNCTION fget({ [pm](OBJ) -> D const & { return *pm; }, api::scope(*this) });
			return def_property_readonly_static(name, fget, ReturnPolicy_Reference, FWD(extra)...);
		}

		template <class Getter, class ... Extra
		> CLASS_ & def_property_readonly(cstring name, Getter const & fget, Extra && ... extra)
		{
			return def_property_readonly(name, CPP_FUNCTION({ api::method_adaptor<type>(fget) }), ReturnPolicy_ReferenceInternal, FWD(extra)...);
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
			return def_property(name, fget, CPP_FUNCTION({ api::method_adaptor<type>(fset) }), FWD(extra)...);
		}

		template <class Getter, class ... Extra
		> CLASS_ & def_property(cstring name, Getter const & fget, CPP_FUNCTION const & fset, Extra && ... extra)
		{
			return def_property(name, CPP_FUNCTION({ api::method_adaptor<type>(fget) }), fset, ReturnPolicy_ReferenceInternal, FWD(extra)...);
		}

		template <class ... Extra
		> CLASS_ & def_property(cstring name, CPP_FUNCTION const & fget, CPP_FUNCTION const & fset, Extra && ... extra)
		{
			return def_property_static(name, fget, fset, api::is_method(*this), FWD(extra)...);
		}

		template <class Getter, class ... Extra
		> CLASS_ & def_property_static(cstring name, Getter const & fget, CPP_FUNCTION const & fset, Extra && ... extra)
		{
			return def_property_static(name, CPP_FUNCTION({ fget }), fset, ReturnPolicy_Reference, FWD(extra)...);
		}
	
		template <class ... Extra
		> CLASS_ & def_property_static(cstring name, CPP_FUNCTION const & fget, CPP_FUNCTION const & fset, Extra && ... extra)
		{
			if (fget) { api::process_attributes<Extra...>::init(***fget, FWD(extra)...); }
	
			if (fset) { api::process_attributes<Extra...>::init(***fset, FWD(extra)...); }

			return add_object(name, PROPERTY({ fget, fset })), (*this);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace impl
	{
		template <class T> struct BindClassHelper final
		{
			constexpr BindClassHelper() noexcept = default;

			NODISCARD constexpr auto operator+(CLASS_<T>(*fn)(CLASS_<T>)) const noexcept
			{
				return static_cast<bindfunc>(static_cast<void *>(fn));
			}
		};
	}

#define MAKE_CLASS_BINDER(m_class, m_var) \
	(ism::impl::BindClassHelper<m_class>{}) + [](ism::CLASS_<m_class> m_var) -> ism::CLASS_<m_class>

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_CLASS_HPP_
