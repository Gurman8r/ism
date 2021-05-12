#include <core/api/object/list_object.hpp>
#include <core/api/modsupport.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

DECLEXPR(CoreList::ob_type_static) = COMPOSE(CoreType, t)
{
	t.tp_name = "list";
	t.tp_basicsize = sizeof(CoreList);
	t.tp_flags = TypeFlags_Default | TypeFlags_List_Subclass;
	t.tp_base = typeof<OBJECT>();

	t.tp_len = (lenfunc)[](OBJECT o) { return (ssize_t)LIST(o)->size(); };

	t.tp_alloc = (allocfunc)[](size_t size) { return memalloc(size); };
	t.tp_free = (freefunc)[](void * ptr) { memdelete((CoreList *)ptr); };

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v)
	{
		if (isinstance<LIST>(v)) return util::compare(***LIST(o), ***LIST(v));
		return util::compare(o.ptr(), v.ptr());
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void CoreList::_bind_methods(CoreType & t)
{
	t.attr("__contains__") = CPP_FUNCTION([](OBJECT self, OBJECT value) -> OBJECT {
		return Core_Bool(LIST(self)->contains(value));
	});
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */