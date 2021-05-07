#include <core/api/types/property_object.hpp>
#include <core/api/modsupport.hpp>

using namespace ism;

DECLEXPR(CoreProperty::ob_class) { nullptr };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static GetSetDef property_getsets[] =
{
	{ /* sentinal */ },
};

void CoreProperty::_bind_class(CoreType & t)
{
	t.tp_name = "property";
	t.tp_basicsize = sizeof(CoreProperty);
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType;

	t.tp_operator_delete = (freefunc)[](void * ptr) { memdelete((CoreProperty *)ptr); };

	t.tp_getsets = property_getsets;

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v)
	{
		return util::compare(o.ptr(), v.ptr());
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */