#include <core/api/object/generic_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_IMPLEMENT_CLASS_TYPE(GenericObject, t)
{
	t.tp_name = "generic_type";

	t.tp_size = sizeof(GenericObject);

	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType | TypeFlags_Type_Subclass;

	t.tp_getattro = (getattrofunc)type_getattro;

	t.tp_setattro = (setattrofunc)type_setattro;

	t.tp_new = (newfunc)[](TYPE type, OBJ args) -> OBJ { return memnew(GenericObject); };

	t.tp_del = (delfunc)[](Object * ptr) { memdelete((GenericObject *)ptr); };
};

void GenericObject::_bind_methods()
{
	CLASS_<GENERIC>()

		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */