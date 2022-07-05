#include <core/object/property_object.hpp>
#include <core/object/detail/class.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(PropertyObject, t, TypeFlags_MethodDescriptor)
	{
		t.tp_descr_get = (descrgetfunc)[](OBJ self, OBJ obj, OBJ cls) -> OBJ
		{
			return ((PROPERTY &)self).get(obj);
		};

		t.tp_descr_set = (descrsetfunc)[](OBJ self, OBJ obj, OBJ value) -> Error_
		{
			return ((PROPERTY &)self).set(obj, value);
		};

		t.tp_install = CLASS_INSTALLER(PropertyObject, t)
		{
			return t;
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}