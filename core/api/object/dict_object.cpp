#include <core/api/object/dict_object.hpp>
#include <core/api/class.hpp>

using namespace ism;
using namespace ism::api;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_STATIC_CLASS_TYPE(DictObject, t)
{
	t.tp_name = "dict";
	t.tp_size = sizeof(DictObject);
	t.tp_flags = TypeFlags_Default | TypeFlags_Dict_Subclass;
	t.tp_base = typeof<OBJECT>();

	t.tp_len = (lenfunc)[](OBJECT o) { return (ssize_t)DICT(o).size(); };

	t.tp_alloc = (allocfunc)[](size_t size) { return memalloc(size); };
	t.tp_free = (freefunc)[](void * ptr) { memdelete((DictObject *)ptr); };

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v) { return util::compare(*o, *v); };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DictObject::_bind_methods(TypeObject & t)
{
	t.tp_dict["__contains__"] = CPP_FUNCTION({ [](OBJECT self, OBJECT value)
	{
		return Core_Bool(DICT(self).contains(value));
	} });
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */