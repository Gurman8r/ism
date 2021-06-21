#ifndef _ISM_PROPERTY_OBJECT_HPP_
#define _ISM_PROPERTY_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// property
namespace ism
{
	// property object
	class ISM_API PropertyObject : public BaseObject
	{
		ISM_OBJECT_DEFAULT(PropertyObject, BaseObject);

	protected:
		static void _bind_class(OBJECT scope);

	public:
		OBJECT m_get{}, m_set{};

		PropertyObject(OBJECT const & fget, OBJECT const & fset) : self_type{} { m_get = fget; m_set = fset; }

		PropertyObject(OBJECT const & fget) : self_type{} { m_get = fget; m_set = nullptr; }

		template <class Getter, class Setter
		> PropertyObject(Getter const & fget, Setter const & fset) : self_type{} { m_get = CPP_FUNCTION({ fget }); m_set = CPP_FUNCTION({ fset }); }

		template <class Getter
		> PropertyObject(Getter const & fget, OBJECT const & fset) : self_type{} { m_get = CPP_FUNCTION({ fget }); m_set = fset; }

		template <class Setter
		> PropertyObject(OBJECT const & fget, Setter const & fset) : self_type{} { m_get = fget; m_set = CPP_FUNCTION({ fset }); }

		template <class Getter
		> PropertyObject(Getter const & fget) : self_type{} { m_get = CPP_FUNCTION({ fget }); m_set = nullptr; }
	};

	// property delete
	template <> struct DefaultDelete<PropertyObject> : DefaultDelete<BaseObject> {};

	// property check
#define ISM_PROPERTY_CHECK(o) (ism::isinstance<PROPERTY>(o))

	// property handle
	template <> class Handle<PropertyObject> : public BaseHandle<PropertyObject>
	{
		ISM_HANDLE_DEFAULT(PropertyObject, ISM_PROPERTY_CHECK);

	public:
		template <class ... Args
		> OBJECT get(Args && ... args) const { return m_ptr->m_get(FWD(args)...); }

		template <class ... Args
		> OBJECT set(Args && ... args) const { return m_ptr->m_set(FWD(args)...); }
	};
}

#endif // !_ISM_PROPERTY_OBJECT_HPP_
