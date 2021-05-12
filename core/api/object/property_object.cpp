#include <core/api/object/property_object.hpp>
#include <core/api/modsupport.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static GetSetDef property_getsets[] =
{
	{ /* sentinal */ },
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

DECLEXPR(CoreProperty::ob_type_static) = COMPOSE(CoreType, t)
{
	t.tp_name = "property";
	t.tp_basicsize = sizeof(CoreProperty);
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType;
	t.tp_base = typeof<OBJECT>();

	t.tp_descr_get = (descrgetfunc)[](OBJECT attr, OBJECT self, OBJECT type) -> OBJECT {
		return nullptr;
	};

	t.tp_descr_set = (descrsetfunc)[](OBJECT attr, OBJECT self, OBJECT type) -> Error {
		return Error_None;
	};

	t.tp_alloc = (allocfunc)[](size_t size) { return memalloc(size); };
	t.tp_free = (freefunc)[](void * ptr) { memdelete((CoreProperty *)ptr); };

	t.tp_getsets = property_getsets;

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v)
	{
		return util::compare(o.ptr(), v.ptr());
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void CoreProperty::_bind_methods(CoreType & t)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */