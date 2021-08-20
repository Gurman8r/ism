#include <core/api/object/dict_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_IMPLEMENT_CLASS_TYPE(DictObject, t)
{
	t.tp_name = "dict";

	t.tp_size = sizeof(DictObject);

	t.tp_flags = TypeFlags_Default | TypeFlags_Dict_Subclass;

	t.tp_len = (lenfunc)[](OBJ self) { return (ssize_t)DICT(self).size(); };

	t.tp_compare = (cmpfunc)[](OBJ self, OBJ other) { return util::compare(*self, *other); };

	t.tp_create = (constructor)[](TYPE type, OBJ args) -> OBJ { return memnew(DictObject); };

	t.tp_destroy = (destructor)[](Object * ptr) { memdelete((DictObject *)ptr); };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DictObject::_bind_methods()
{
	CLASS_<DICT>()

		.def(init<>())

		.def("__contains__", &DICT::contains<OBJ const &>)

		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */