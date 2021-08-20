#include <core/api/object/capsule_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_IMPLEMENT_CLASS_TYPE(CapsuleObject, t)
{
	t.tp_name = "capsule";

	t.tp_size = sizeof(CapsuleObject);

	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType;

	t.tp_compare = (cmpfunc)[](OBJ self, OBJ other) { return util::compare(*self, *other); };

	t.tp_create = (constructor)[](TYPE type, OBJ args) -> OBJ { return memnew(CapsuleObject); };

	t.tp_destroy = (destructor)[](Object * ptr) { memdelete((CapsuleObject *)ptr); };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void CapsuleObject::_bind_methods()
{
	CLASS_<CAPSULE>()
	
		.def(init<>())
	
		.def_property("__name__", [](CAPSULE self) { return self->m_name; }, [](CAPSULE self, STR value) { self->m_name = value; })
	
		.def_property("__doc__", [](CAPSULE self) { return self->m_doc; }, [](CAPSULE self, STR value) { self->m_doc = value; })
	
		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */