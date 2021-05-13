#include <core/api/modsupport.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

DECLEXPR(CoreDict::ob_type_static) = COMPOSE(CoreType, t)
{
	t.tp_name = "dict";
	t.tp_basicsize = sizeof(CoreDict);
	t.tp_flags = TypeFlags_Default | TypeFlags_Dict_Subclass;
	t.tp_base = typeof<OBJECT>();

	t.tp_len = (lenfunc)[](OBJECT o) { return (ssize_t)DICT(o).size(); };

	t.tp_alloc = (allocfunc)[](size_t size) { return memalloc(size); };
	t.tp_free = (freefunc)[](void * ptr) { memdelete((CoreDict *)ptr); };

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v) { return util::compare(*o, *v); };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void CoreDict::_bind_methods(CoreType & t)
{
	t.attr("__contains__") = CPP_FUNCTION([](OBJECT self, OBJECT value) -> OBJECT {
		return Core_Bool(DICT(self)->contains(value));
	});
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */