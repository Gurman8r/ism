#include <core/api/types/capsule_object.hpp>
#include <core/api/modsupport.hpp>

using namespace ism;

DECLEXPR(CoreCapsule::ob_class) { nullptr };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static GetSetDef capsule_getsets[] =
{
	{ "__name__",
		(getter)[](OBJECT self, void *) -> OBJECT { return STR((***CAPSULE(self)).name); },
		(setter)[](OBJECT self, OBJECT value, void *) -> Error { return ((***CAPSULE(self)).name = ***STR(value)), Error_None; },
	},
	{ "__doc__",
		(getter)[](OBJECT self, void *) -> OBJECT { return STR((***CAPSULE(self)).doc); },
		(setter)[](OBJECT self, OBJECT value, void *) -> Error { return ((***CAPSULE(self)).doc = ***STR(value)), Error_None; },
	},
	{ /* sentinal */ },
};

void CoreCapsule::_bind_class(CoreType & t)
{
	t.tp_name = "capsule";
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType;

	t.tp_operator_delete = (freefunc)[](void * ptr) { memdelete((CoreCapsule *)ptr); };

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v)
	{
		return util::compare(o.ptr(), v.ptr());
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */