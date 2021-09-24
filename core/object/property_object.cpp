#include <core/object/property_object.hpp>
#include <core/detail/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJ_IMPL(PropertyObject, t, TypeFlags_BaseType | TypeFlags_MethodDescriptor)
{
	t.tp_descr_get = (descrgetfunc)[](OBJ self, OBJ obj, OBJ type) -> OBJ
	{
		return PROPERTY(self).get(obj);
	};

	t.tp_descr_set = (descrsetfunc)[](OBJ self, OBJ obj, OBJ value) -> Error
	{
		return PROPERTY(self).set(obj, value);
	};

	t.tp_bind = CLASS_BINDER(PropertyObject, c)
	{
		return c;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */