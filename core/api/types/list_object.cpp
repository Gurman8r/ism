#include <core/api/types/list_object.hpp>
#include <core/api/modsupport.hpp>

using namespace ism;

DECLEXPR(CoreList::ob_class) { nullptr };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static MethodDef list_methods[] =
{
	{ "__contains__", (cfunction)[](OBJECT self, OBJECT value) -> OBJECT {
		return Core_Bool(LIST(self)->contains(value));
	} },
	{ /* sentinal */ },
};

void CoreList::_bind_class(CoreType & t)
{
	t.tp_name = "list";
	t.tp_basicsize = sizeof(CoreList);
	t.tp_flags = TypeFlags_Default | TypeFlags_List_Subclass;

	t.tp_len = (lenfunc)[](OBJECT o) { return (ssize_t)LIST(o)->size(); };

	t.tp_operator_delete = (freefunc)[](void * ptr) { memdelete((CoreList *)ptr); };

	t.tp_methods = list_methods;

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v)
	{
		if (isinstance<LIST>(v)) return util::compare(***LIST(o), ***LIST(v));
		return util::compare(o.ptr(), v.ptr());
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */