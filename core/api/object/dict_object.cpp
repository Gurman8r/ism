#include <core/api/object/dict_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_OBJECT_TYPE_STATIC(DictObject, t)
{
	t.tp_flags = TypeFlags_Default | TypeFlags_Dict_Subclass;

	t.tp_len = (lenfunc)[](OBJECT o) { return (ssize_t)DICT(o).size(); };

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v) { return util::compare(*o, *v); };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DictObject::_bind_class(OBJECT scope)
{
	CLASS_<DICT>(scope, "dict", get_type_static())

		.def(init<>())

		.def("__contains__", &DICT::contains<OBJECT const &>)

		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */