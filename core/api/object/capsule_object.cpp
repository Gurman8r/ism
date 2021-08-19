#include <core/api/object/capsule_object.hpp>
#include <servers/script_server.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_CLASS_IMPLEMENTATION(CapsuleObject, t)
{
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType;

	t.tp_compare = (cmpfunc)[](OBJ self, OBJ other) { return util::compare(*self, *other); };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void CapsuleObject::_bind_class(OBJ scope)
{
	CLASS_<CAPSULE>(scope, "capsule")

		.def(init<>())

		.def_property("__name__", [](CAPSULE self) { return self->m_name; }, [](CAPSULE self, STR value) { self->m_name = value; })

		.def_property("__doc__", [](CAPSULE self) { return self->m_doc; }, [](CAPSULE self, STR value) { self->m_doc = value; })

		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */