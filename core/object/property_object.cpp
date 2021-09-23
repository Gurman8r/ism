#include <core/object/property_object.hpp>
#include <core/object/detail/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJ_CLASS_IMPL(PropertyObject, t, "property", TypeFlags_BaseType | TypeFlags_MethodDescriptor)
{
	t.tp_new = (newfunc)[](TYPE type, OBJ args) -> OBJ { return memnew(PropertyObject); };

	t.tp_descr_get = (descrgetfunc)[](OBJ self, OBJ obj, OBJ type) -> OBJ
	{
		return PROPERTY(self).get(obj);
	};

	t.tp_descr_set = (descrsetfunc)[](OBJ self, OBJ obj, OBJ value) -> Error
	{
		return PROPERTY(self).set(obj, value);
	};

	t.tp_bind = (bindfunc)[](TYPE type) -> TYPE
	{
		return CLASS_<PropertyObject>(type);
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */