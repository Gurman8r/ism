#include <core/api/object/dict_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_OBJECT_TYPE_STATIC(DictObject, t)
{
	t.tp_name = "dict";
	t.tp_size = sizeof(DictObject);
	t.tp_flags = TypeFlags_Default | TypeFlags_Dict_Subclass;
	t.tp_base = typeof<OBJECT>();

	t.tp_len = (lenfunc)[](OBJECT o) { return (ssize_t)DICT(o).size(); };

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v) { return util::compare(*o, *v); };

	t.tp_free = (freefunc)[](void * ptr) { memdelete((DictObject *)ptr); };

	t.tp_alloc = (allocfunc)[](size_t size) { return memalloc(size); };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DictObject::_bind_class(TypeObject & t)
{
	CLASS_<DICT>(&t, "dict")

		//.def(init<>())

		.def("__contains__", [](DICT self, OBJECT value) { return DICT(self).contains(value); })

		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */