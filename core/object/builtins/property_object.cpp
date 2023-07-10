#include <core/object/builtins/property_object.hpp>
#include <core/object/class.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(PropertyObject, t, TypeFlags_MethodDescriptor)
	{
		t.tp_descr_get = (DescrGetFunc)[](ObjectRef self, ObjectRef obj, ObjectRef cls) -> ObjectRef
		{
			return ((PropertyRef &)self).get(obj);
		};

		t.tp_descr_set = (DescrSetFunc)[](ObjectRef self, ObjectRef obj, ObjectRef value) -> Error_
		{
			return ((PropertyRef &)self).set(obj, value);
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}