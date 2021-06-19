#include <core/api/object/capsule_object.hpp>
#include <core/api/class.hpp>

using namespace ism;
using namespace ism::api;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_STATIC_CLASS_TYPE(CapsuleObject, t)
{
	t.tp_name = "capsule";
	t.tp_size = sizeof(CapsuleObject);
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType;
	t.tp_base = typeof<OBJECT>();

	t.tp_alloc = (allocfunc)[](size_t size) { return memalloc(size); };
	t.tp_free = (freefunc)[](void * ptr) { memdelete((CapsuleObject *)ptr); };

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v) { return util::compare(*o, *v); };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void CapsuleObject::_bind_methods(TypeObject & t)
{
	t.tp_dict["__name__"] = PROPERTY({
		[](CAPSULE self) { return self->m_name; },
		[](CAPSULE self, STR value) { self->m_name = value; } });

	t.tp_dict["__doc__"] = PROPERTY({
		[](CAPSULE self) { return self->m_doc; },
		[](CAPSULE self, STR value) { self->m_doc = value; } });
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */