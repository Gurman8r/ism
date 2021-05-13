#include <core/api/modsupport.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static GetSetDef capsule_getsets[] =
{
	{ "__name__",
		(getter)[](OBJECT self, void *) -> OBJECT { return STR(CAPSULE(self).get_name()); },
		(setter)[](OBJECT self, OBJECT value, void *) -> Error { return CAPSULE(self).set_name(***STR(value)), Error_None; },
	},
	{ "__doc__",
		(getter)[](OBJECT self, void *) -> OBJECT { return STR(CAPSULE(self).get_doc()); },
		(setter)[](OBJECT self, OBJECT value, void *) -> Error { return CAPSULE(self).set_doc(***STR(value)), Error_None; },
	},
	{ /* sentinal */ },
};

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
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */