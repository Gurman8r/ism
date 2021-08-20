#ifndef _ISM_PROPERTY_OBJECT_HPP_
#define _ISM_PROPERTY_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// property
namespace ism
{
	// property object
	class ISM_API PropertyObject : public Object
	{
		ISM_OBJECT(PropertyObject, Object);

	protected:
		static void _bind_methods();

	public:
		OBJ m_get{}, m_set{};

		PropertyObject() : Object{ get_class_static() } {}

		PropertyObject(OBJ const & fget, OBJ const & fset) : PropertyObject{} { m_get = fget; m_set = fset; }

		PropertyObject(OBJ const & fget) : PropertyObject{} { m_get = fget; m_set = nullptr; }

		template <class Getter, class Setter
		> PropertyObject(Getter const & fget, Setter const & fset) : PropertyObject{} { m_get = CPP_FUNCTION({ fget }); m_set = CPP_FUNCTION({ fset }); }

		template <class Getter
		> PropertyObject(Getter const & fget, OBJ const & fset) : PropertyObject{} { m_get = CPP_FUNCTION({ fget }); m_set = fset; }

		template <class Setter
		> PropertyObject(OBJ const & fget, Setter const & fset) : PropertyObject{} { m_get = fget; m_set = CPP_FUNCTION({ fset }); }

		template <class Getter
		> PropertyObject(Getter const & fget) : PropertyObject{} { m_get = CPP_FUNCTION({ fget }); m_set = nullptr; }
	};

	// property delete
	template <> struct DefaultDelete<PropertyObject> : DefaultDelete<Object> {};

	// property check
#define ISM_CHECK_PROPERTY(o) (ism::isinstance<ism::PROPERTY>(o))

	// property handle
	template <> class Handle<PropertyObject> : public Ref<PropertyObject>
	{
		ISM_HANDLE(PropertyObject, ISM_CHECK_PROPERTY);

	public:
		template <class ... Args
		> OBJ get(Args && ... args) const { return m_ptr->m_get(FWD(args)...); }

		template <class ... Args
		> OBJ set(Args && ... args) const { return m_ptr->m_set(FWD(args)...); }
	};
}

#endif // !_ISM_PROPERTY_OBJECT_HPP_
