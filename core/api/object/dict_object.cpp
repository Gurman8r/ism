#include <core/api/object/dict_object.hpp>
#include <core/api/modsupport.hpp>

using namespace ism;
using namespace ism::api;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

STATIC_MEMBER(DictObject::ob_type_static) = COMPOSE(TypeObject, t)
{
	t.tp_name = "dict";
	t.tp_basicsize = sizeof(DictObject);
	t.tp_flags = TypeFlags_Default | TypeFlags_Dict_Subclass;
	t.tp_base = typeof<OBJECT>();

	t.tp_len = (lenfunc)[](OBJECT o) { return (ssize_t)DICT(o).size(); };

	t.tp_alloc = (allocfunc)[](size_t size) { return memalloc(size); };
	t.tp_free = (freefunc)[](void * ptr) { memdelete((DictObject *)ptr); };

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v) { return util::compare(*o, *v); };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DictObject::_bind_class(TypeObject & t)
{
	t.attr("__contains__") = CPP_FUNCTION([](OBJECT self, OBJECT value) -> OBJECT {
		return Core_Bool(DICT(self)->contains(value));
	});
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */