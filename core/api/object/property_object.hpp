#ifndef _ISM_PROPERTY_OBJECT_HPP_
#define _ISM_PROPERTY_OBJECT_HPP_

#include <core/api/object/base_object.hpp>

// property object
class ISM_API ism::api::PropertyObject : public BaseObject
{
	ISM_OBJECT(PropertyObject, BaseObject);

protected:
	static void _bind_class(TypeObject & t);

	OBJECT m_get{}, m_set{};

public:
	PropertyObject(OBJECT fget, OBJECT fset) : self_type{} { m_get = fget; m_set = fset; }

	PropertyObject(OBJECT fget) : self_type{} { m_get = fget; m_set = nullptr; }
};

// property deleter
template <> struct ism::DefaultDelete<ism::api::PropertyObject> : ism::DefaultDelete<ism::api::BaseObject> {};

// property handle
template <> class ism::api::Handle<ism::api::PropertyObject> : public ism::api::BaseHandle<ism::api::PropertyObject>
{
	ISM_HANDLE(PropertyObject);

public:
	Handle() = default;

	~Handle() = default;

	OBJECT get(OBJECT obj) const { return m_ref->m_get(obj); }

	Error set(OBJECT obj, OBJECT value) const { return m_ref->m_set(obj, value), Error_None; }
};

#endif // !_ISM_PROPERTY_OBJECT_HPP_
