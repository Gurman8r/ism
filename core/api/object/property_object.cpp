#include <core/api/object/property_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBEDED_CLASS(PropertyObject, t, TypeFlags_MethodDescriptor)
{
	t.tp_descr_get = (descrgetfunc)[](OBJ self, OBJ obj, OBJ cls) -> OBJ
	{
		return ((PROPERTY &)self).get(obj);
	};

	t.tp_descr_set = (descrsetfunc)[](OBJ self, OBJ obj, OBJ value) -> Error
	{
		return ((PROPERTY &)self).set(obj, value);
	};

	CLASS_DEF(PropertyObject, t)
	{
		return t;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */