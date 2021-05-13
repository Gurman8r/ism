#include <core/api/modsupport.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

DECLEXPR(CoreCapsule::ob_type_static) = COMPOSE(CoreType, t)
{
	t.tp_name = "capsule";
	t.tp_basicsize = sizeof(CoreCapsule);
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType;
	t.tp_base = typeof<OBJECT>();

	t.tp_alloc = (allocfunc)[](size_t size) { return memalloc(size); };
	t.tp_free = (freefunc)[](void * ptr) { memdelete((CoreCapsule *)ptr); };

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v) { return util::compare(*o, *v); };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void CoreCapsule::_bind_class(CoreType & t)
{
	t.attr("__name__") = PROPERTY({
		CPP_FUNCTION([](CAPSULE self) { return self->m_capsule.name; }),
		CPP_FUNCTION([](CAPSULE self, STR value) { self->m_capsule.name = value; })
	});

	t.attr("__doc__") = PROPERTY({
		CPP_FUNCTION([](CAPSULE self) { return self->m_capsule.doc; }),
		CPP_FUNCTION([](CAPSULE self, STR value) { self->m_capsule.doc = value; })
	});
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */