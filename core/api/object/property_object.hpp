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
		static void _bind_class(TypeObject & t);

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
}

// property delete
namespace ism { template <> struct DefaultDelete<api::PropertyObject> : DefaultDelete<api::BaseObject> {}; }

// property check
#define ISM_PROPERTY_CHECK(o) (isinstance<PROPERTY>(o))

// property handle
namespace ism::api
{
	template <> class Handle<PropertyObject> : public BaseHandle<PropertyObject>
	{
		ISM_HANDLE_DEFAULT(PropertyObject, ISM_PROPERTY_CHECK);

	public:
	};
}

#endif // !_ISM_PROPERTY_OBJECT_HPP_
