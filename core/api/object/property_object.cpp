#include <core/api/object/property_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

IMPLEMENT_CLASS(PropertyObject, t)
{
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType | TypeFlags_MethodDescriptor;
	
	t.tp_compare = (cmpfunc)[](OBJ self, OBJ other) { return CMP(*self, *other); };

	t.tp_descr_get = (descrgetfunc)[](OBJ self, OBJ obj, OBJ type) -> OBJ
	{
		return PROPERTY(self).get(obj);
	};

	t.tp_descr_set = (descrsetfunc)[](OBJ self, OBJ obj, OBJ value) -> Error
	{
		return PROPERTY(self).set(obj, value), Error_None;
	};

	t.tp_new = (newfunc)[](TYPE type, OBJ args) -> OBJ
	{
		return holder_type::new_();
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void PropertyObject::_bind_class(OBJ scope)
{
	CLASS_<PROPERTY>(scope, "property", get_class())
		.def(init<>())
		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */