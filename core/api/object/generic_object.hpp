#ifndef _ISM_GENERIC_OBJECT_HPP_
#define _ISM_GENERIC_OBJECT_HPP_

#include <core/api/object/module_object.hpp>

// generic
namespace ism
{
	// generic object
	class ISM_API GenericObject : public Object
	{
		ISM_OBJECT(GenericObject, Object);

	protected:
		static void _bind_class(OBJ scope);

	public:
		GenericObject() : Object{ get_class() } {}
	};

	// generic delete
	template <> struct DefaultDelete<GenericObject> : DefaultDelete<Object> {};

	// generic check
#define ISM_CHECK_GENERIC(o) (ism::isinstance<ism::GENERIC>(o))

	// generic handle
	template <> class Handle<GenericObject> : public Ref<GenericObject>
	{
		ISM_HANDLE(GenericObject, ISM_CHECK_GENERIC);

	public:
	};
}

#endif // !_ISM_GENERIC_OBJECT_HPP_
