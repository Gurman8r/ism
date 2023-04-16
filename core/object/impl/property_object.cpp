#include <core/object/impl/property_object.hpp>
#include <core/object/class.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(PropertyObject, t, TypeFlags_MethodDescriptor)
	{
		t.tp_descr_get = (descrgetfunc)[](OBJ self, OBJ obj, OBJ cls) -> OBJ
		{
			return ((PROPERTY &)self).get(obj);
		};

		t.tp_descr_set = (descrsetfunc)[](OBJ self, OBJ obj, OBJ value) -> Error_
		{
			return ((PROPERTY &)self).set(obj, value);
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}