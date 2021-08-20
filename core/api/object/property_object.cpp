#include <core/api/object/property_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_IMPLEMENT_CLASS_TYPE(PropertyObject, t)
{
	t.tp_name = "property";

	t.tp_size = sizeof(PropertyObject);

	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType | TypeFlags_MethodDescriptor;

	t.tp_compare = (cmpfunc)[](OBJ self, OBJ other) { return util::compare(*self, *other); };

	t.tp_descr_get = (descrgetfunc)[](OBJ self, OBJ obj, OBJ type) -> OBJ
	{
		return PROPERTY(self).get(obj);
	};

	t.tp_descr_set = (descrsetfunc)[](OBJ self, OBJ obj, OBJ value) -> Error
	{
		return PROPERTY(self).set(obj, value), Error_None;
	};

	t.tp_create = (constructor)[](TYPE type, OBJ args) -> OBJ { return memnew(PropertyObject); };

	t.tp_destroy = (destructor)[](Object * ptr) { memdelete((PropertyObject *)ptr); };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void PropertyObject::_bind_methods()
{
	CLASS_<PROPERTY>()

		.def(init<>())

		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */