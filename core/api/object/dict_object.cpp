#include <core/api/object/dict_object.hpp>
#include <servers/script_server.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_CLASS_IMPLEMENTATION(DictObject, t)
{
	t.tp_flags = TypeFlags_Default | TypeFlags_Dict_Subclass;

	t.tp_len = (lenfunc)[](OBJ self) { return (ssize_t)DICT(self).size(); };

	t.tp_compare = (cmpfunc)[](OBJ self, OBJ other) { return util::compare(*self, *other); };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DictObject::_bind_class(OBJ scope)
{
	CLASS_<DICT>(scope, "dict")

		.def(init<>())

		.def("__contains__", &DICT::contains<OBJ const &>)

		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */