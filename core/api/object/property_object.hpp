#ifndef _ISM_PROPERTY_OBJECT_HPP_
#define _ISM_PROPERTY_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// property object
namespace ism::api
{
	class ISM_API PropertyObject : public BaseObject
	{
		ISM_OBJECT_DEFAULT(PropertyObject, BaseObject);

	protected:
		static void _bind_methods(TypeObject & t);

	public:
		OBJECT m_get{}, m_set{};

		PropertyObject(OBJECT fget, OBJECT fset) : self_type{} { m_get = fget; m_set = fset; }

		PropertyObject(OBJECT fget) : self_type{} { m_get = fget; m_set = nullptr; }
	};
}

// property deleter
namespace ism { template <> struct DefaultDelete<api::PropertyObject> : DefaultDelete<api::BaseObject> {}; }

// property handle
namespace ism::api
{
	template <> class Handle<PropertyObject> : public BaseHandle<PropertyObject>
	{
		ISM_HANDLE_DEFAULT(PropertyObject);

	public:
		Handle() = default;

		~Handle() = default;

		OBJECT get(OBJECT obj) const { return m_ref->m_get(obj); }

		Error set(OBJECT obj, OBJECT value) const { return m_ref->m_set(obj, value), Error_None; }
	};
}

#endif // !_ISM_PROPERTY_OBJECT_HPP_
