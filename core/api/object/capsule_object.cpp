#include <core/api/object/capsule_object.hpp>
#include <core/api/modsupport.hpp>

using namespace ism;
using namespace ism::api;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

STATIC_MEMBER(CapsuleObject::ob_type_static) = COMPOSE(TypeObject, t)
{
	t.tp_name = "capsule";
	t.tp_basicsize = sizeof(CapsuleObject);
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType;
	t.tp_base = typeof<OBJECT>();

	t.tp_alloc = (allocfunc)[](size_t size) { return memalloc(size); };
	t.tp_free = (freefunc)[](void * ptr) { memdelete((CapsuleObject *)ptr); };

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v) { return util::compare(*o, *v); };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void CapsuleObject::_bind_class(TypeObject & t)
{
	setitem(t.tp_dict, "__name__", PROPERTY({
		CPP_FUNCTION([](CAPSULE self) { return self->m_name; }),
		CPP_FUNCTION([](CAPSULE self, STR value) { self->m_name = value; })
		}));
	
	setitem(t.tp_dict, "__doc__", PROPERTY({
		CPP_FUNCTION([](CAPSULE self) { return self->m_doc; }),
		CPP_FUNCTION([](CAPSULE self, STR value) { self->m_doc = value; })
		}));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */