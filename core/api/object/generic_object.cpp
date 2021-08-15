#include <core/api/object/generic_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

IMPLEMENT_CLASS_TYPE(GenericObject, t)
{
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType | TypeFlags_Type_Subclass;

	t.tp_getattro = (getattrofunc)type_getattr;

	t.tp_setattro = (setattrofunc)type_setattr;

	t.tp_compare = (cmpfunc)[](OBJECT self, OBJECT other) { return util::compare(*self, *other); };

	t.tp_new = (newfunc)[](TYPE type, OBJECT args) -> OBJECT
	{
		return holder_type::new_();
	};
};

void GenericObject::_bind_class(OBJECT scope)
{
	CLASS_<GENERIC>(scope, "generic_type", get_type_static())

		.def(init<>())

		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */