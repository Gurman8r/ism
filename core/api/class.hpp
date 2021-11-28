#ifndef _ISM_CLASS_HPP_
#define _ISM_CLASS_HPP_

#include <core/api/object/module_object.hpp>

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
		> CLASS_ & def(initimpl::Constructor<Args...> && init, Extra && ... extra)
		{
			return FWD(init).execute(*this, FWD(extra)...); // init.hpp
		}

		template <class ... Args, class ... Extra
		> CLASS_ & def(initimpl::Factory<Args...> && init, Extra && ... extra)
		{
			return FWD(init).execute(*this, FWD(extra)...); // init.hpp
		}

		template <class Func, class ... Extra
		> CLASS_ & def(cstring name, Func && func, Extra && ... extra)
		{
			CPP_FUNCTION cf({
				method_adaptor<type>(FWD(func)),
				attr::name(name),
				attr::is_method(*this),
				attr::sibling(getattr(*this, name, nullptr)),
				FWD(extra)... });
			return add_object(cf.name(), cf), (*this);
		}

		template <class Func, class ... Extra
		> CLASS_ & def_static(cstring name, Func && func, Extra && ... extra)
		{
			CPP_FUNCTION cf({
				FWD(func),
				attr::name(name),
				attr::scope(*this),
				attr::sibling(getattr(*this, name, nullptr)),
				FWD(extra)... });
			return add_object(cf.name(), cf), (*this);
		}

		template <class C, class D, class ... Extra
		> CLASS_ & def_readwrite(cstring name, D C::*pm, Extra && ... extra)
		{
			static_assert(std::is_same_v<C, type> || std::is_base_of_v<C, type>, "def_readwrite() requires a class member (or base class member)");
			CPP_FUNCTION fget({ [pm](type const & c) -> D const & { return c.*pm; }, attr::is_method(*this) });
			CPP_FUNCTION fset({ [pm](type & c, D const & value) { c.*pm = value; }, attr::is_method(*this) });
			return def_property(name, fget, fset, ReturnValuePolicy_ReferenceInternal, FWD(extra)...);
		}

		template <class C, class D, class ... Extra
		> CLASS_ & def_readonly(cstring name, D const C::*pm, Extra && ... extra)
		{
			static_assert(std::is_same_v<C, type> || std::is_base_of_v<C, type>, "def_readonly() requires a class member (or base class member)");
			CPP_FUNCTION fget({ [pm](type const & c) -> D const & { return c.*pm; }, attr::is_method(*this) });
			return def_property_readonly(name, fget, ReturnValuePolicy_ReferenceInternal, FWD(extra)...);
		}

		template <class D, class ... Extra
		> CLASS_ & def_readwrite_static(cstring name, D * pm, Extra && ... extra)
		{
			CPP_FUNCTION fget({ [pm](OBJ) -> D const & { return *pm; }, attr::scope(*this) });
			CPP_FUNCTION fset({ [pm](OBJ, D const & value) { *pm = value; }, attr::scope(*this) });
			return def_property_static(name, fget, fset, ReturnValuePolicy_Reference, FWD(extra)...);
		}

		template <class D, class ... Extra
		> CLASS_ & def_readonly_static(cstring name, D const * pm, Extra && ... extra)
		{
			CPP_FUNCTION fget({ [pm](OBJ) -> D const & { return *pm; }, attr::scope(*this) });
			return def_property_readonly_static(name, fget, ReturnValuePolicy_Reference, FWD(extra)...);
		}

		template <class Getter, class ... Extra
		> CLASS_ & def_property_readonly(cstring name, Getter const & fget, Extra && ... extra)
		{
			return def_property_readonly(name, CPP_FUNCTION({ method_adaptor<type>(fget) }), ReturnValuePolicy_ReferenceInternal, FWD(extra)...);
		}

		template <class ... Extra
		> CLASS_ & def_property_readonly(cstring name, CPP_FUNCTION const & fget, Extra && ... extra)
		{
			return def_property(name, fget, nullptr, FWD(extra)...);
		}

		template <class Getter, class ... Extra
		> CLASS_ & def_property_readonly_static(cstring name, Getter const & fget, Extra && ... extra)
		{
			return def_property_readonly_static(name, CPP_FUNCTION({ fget }), ReturnValuePolicy_Reference, FWD(extra)...);
		}

		template <class ... Extra
		> CLASS_ & def_property_readonly_static(cstring name, CPP_FUNCTION const & fget, Extra && ... extra)
		{
			return def_property_static(name, fget, nullptr, FWD(extra)...);
		}

		template <class Getter, class Setter, class ... Extra
		> CLASS_ & def_property(cstring name, Getter const & fget, Setter const & fset, Extra && ... extra)
		{
			return def_property(name, fget, CPP_FUNCTION({ method_adaptor<type>(fset) }), FWD(extra)...);
		}

		template <class Getter, class ... Extra
		> CLASS_ & def_property(cstring name, Getter const & fget, CPP_FUNCTION const & fset, Extra && ... extra)
		{
			return def_property(name, CPP_FUNCTION({ method_adaptor<type>(fget) }), fset, ReturnValuePolicy_ReferenceInternal, FWD(extra)...);
		}

		template <class ... Extra
		> CLASS_ & def_property(cstring name, CPP_FUNCTION const & fget, CPP_FUNCTION const & fset, Extra && ... extra)
		{
			return def_property_static(name, fget, fset, attr::is_method(*this), FWD(extra)...);
		}

		template <class Getter, class ... Extra
		> CLASS_ & def_property_static(cstring name, Getter const & fget, CPP_FUNCTION const & fset, Extra && ... extra)
		{
			return def_property_static(name, CPP_FUNCTION({ fget }), fset, ReturnValuePolicy_Reference, FWD(extra)...);
		}
	
		template <class ... Extra
		> CLASS_ & def_property_static(cstring name, CPP_FUNCTION const & fget, CPP_FUNCTION const & fset, Extra && ... extra)
		{
			if (fget) { attr::process_attributes<Extra...>::init(***fget, FWD(extra)...); }
	
			if (fset) { attr::process_attributes<Extra...>::init(***fset, FWD(extra)...); }

			return add_object(name, PROPERTY({ fget, fset })), (*this);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace impl
	{
		template <class T> struct ClassBinderHelper final
		{
			constexpr ClassBinderHelper() noexcept = default;

			NODISCARD constexpr auto operator+(CLASS_<T>(*fn)(CLASS_<T>)) const noexcept
			{
				return static_cast<bindfunc>(static_cast<void *>(fn));
			}
		};
	}

#define CLASS_BINDER(m_class, m_var) \
	(impl::ClassBinderHelper<m_class>{}) + [](CLASS_<m_class> m_var) -> CLASS_<m_class>

#define CLASS_DEFINITION(m_class, m_var) \
	m_var.tp_bind = CLASS_BINDER(m_class, m_var)

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_CLASS_HPP_
