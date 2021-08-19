#include <core/api/object/generic_object.hpp>
#include <servers/script_server.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_CLASS_IMPLEMENTATION(GenericObject, t)
{
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType | TypeFlags_Type_Subclass;

	t.tp_getattro = (getattrofunc)type_getattro;

	t.tp_setattro = (setattrofunc)type_setattro;

	t.tp_compare = (cmpfunc)[](OBJ self, OBJ other) { return util::compare(*self, *other); };
};

void GenericObject::_bind_class(OBJ scope)
{
	CLASS_<GENERIC>(scope, "generic_type")

		.def(init<>())

		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */