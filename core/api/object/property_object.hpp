#ifndef _ISM_PROPERTY_OBJECT_HPP_
#define _ISM_PROPERTY_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// property
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace detail
	{
		struct NODISCARD property_record
		{
			OBJECT get{};
			OBJECT set{};
			void * closure{};
			String name{}, doc{};
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class NODISCARD ISM_API CoreProperty : public CoreObject
	{
		ISM_OBJECT(CoreProperty, CoreObject);

	protected:
		static void _bind_methods(CoreType & t);

	public:
		detail::property_record m_property{};

		using storage_type = decltype(m_property);

		CoreProperty(OBJECT fget, OBJECT fset, void * closure = nullptr) : base_type{ &ob_type_static }
		{
			m_property.get = fget;
			m_property.set = fset;
			m_property.closure = closure;
		}

		NODISCARD auto & operator*() const { return const_cast<storage_type &>(m_property); }

		NODISCARD auto * operator->() const { return const_cast<storage_type *>(&m_property); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> class Handle<CoreProperty> : public BaseHandle<CoreProperty>
	{
		ISM_HANDLE(CoreProperty);

	public:
		Handle() = default;

		~Handle() = default;

		using storage_type = CoreProperty::storage_type;

		NODISCARD OBJECT get(OBJECT o) const { return (*m_ref)->get(o); }

		Error set(OBJECT o, OBJECT v) const { return (*m_ref)->set(o, v), Error_None; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_PROPERTY_OBJECT_HPP_
