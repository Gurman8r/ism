#include <core/api/types/dict_object.hpp>
#include <core/api/modsupport.hpp>

using namespace ism;

DECLEXPR(CoreDict::ob_class) { nullptr };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static MethodDef dict_methods[] =
{
	{ "__contains__", (cfunction)[](OBJECT self, OBJECT value) -> OBJECT {
		return Core_Bool(DICT(self)->contains(value));
	} },
	{ /* sentinal */ },
};

void CoreDict::_bind_class(CoreType & t)
{
	t.tp_name = "dict";
	t.tp_basicsize = sizeof(CoreDict);
	t.tp_flags = TypeFlags_Default | TypeFlags_Dict_Subclass;

	t.tp_len = (lenfunc)[](OBJECT o) { return (ssize_t)DICT(o)->size(); };

	t.tp_operator_delete = (freefunc)[](void * ptr) { memdelete((CoreDict *)ptr); };

	t.tp_methods = dict_methods;

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v)
	{
		return util::compare(o.ptr(), v.ptr());
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */