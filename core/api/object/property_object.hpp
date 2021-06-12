#ifndef _ISM_PROPERTY_OBJECT_HPP_
#define _ISM_PROPERTY_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// property object
class NODISCARD ISM_API ism::api::PropertyObject : public BaseObject
{
	ISM_OBJECT(PropertyObject, BaseObject);

protected:
	static void _bind_methods(TypeObject & t);

public:
	OBJECT m_get{};
	OBJECT m_set{};
	void * m_closure{};
	String m_name{}, m_doc{};

public:
	PropertyObject(OBJECT fget, OBJECT fset, void * closure = nullptr) : base_type{ &ob_type_static }
	{
		m_get = fget;
		m_set = fset;
		m_closure = closure;
	}
};

// property deleter
template <> struct ism::DefaultDelete<ism::api::PropertyObject> : DefaultDelete<ism::api::BaseObject> {};

// property handle
template <> class ism::api::Handle<ism::api::PropertyObject> : public BaseHandle<PropertyObject>
{
	ISM_HANDLE(PropertyObject);

public:
	Handle() = default;

	~Handle() = default;

	NODISCARD OBJECT get(OBJECT o) const { return (*m_ref).m_get(o); }

	Error set(OBJECT o, OBJECT v) const { return (*m_ref).m_set(o, v), Error_None; }
};

#endif // !_ISM_PROPERTY_OBJECT_HPP_
