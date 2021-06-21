#include <core/api/object/capsule_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_OBJECT_TYPE_STATIC(CapsuleObject, t)
{
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType;

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v) { return util::compare(*o, *v); };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void CapsuleObject::_bind_class(OBJECT scope)
{
	CLASS_<CAPSULE>(scope, "capsule", get_type_static())

		.def(init<>())

		.def_property("__name__", [](CAPSULE self) { return self->m_name; }, [](CAPSULE self, STR value) { self->m_name = value; })

		.def_property("__doc__", [](CAPSULE self) { return self->m_doc; }, [](CAPSULE self, STR value) { self->m_doc = value; })

		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */