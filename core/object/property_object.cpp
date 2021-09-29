#include <core/object/property_object.hpp>
#include <core/object/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_IMPL(PropertyObject, t, TypeFlags_BaseType | TypeFlags_MethodDescriptor)
{
	t.tp_descr_get = (descrgetfunc)[](OBJ self, OBJ obj, OBJ cls) -> OBJ
	{
		return PROPERTY(self).get(obj);
	};

	t.tp_descr_set = (descrsetfunc)[](OBJ self, OBJ obj, OBJ value) -> Error
	{
		return PROPERTY(self).set(obj, value);
	};

	t.tp_bind = MAKE_CLASS_BINDER(PropertyObject, t)
	{
		return t;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */