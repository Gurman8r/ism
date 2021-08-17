#include <core/api/object/dict_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

IMPLEMENT_CLASS(DictObject, t)
{
	t.tp_flags = TypeFlags_Default | TypeFlags_Dict_Subclass;

	t.tp_len = (lenfunc)[](OBJ self) { return (ssize_t)DICT(self).size(); };

	t.tp_compare = (cmpfunc)[](OBJ self, OBJ other) { return CMP(*self, *other); };

	t.tp_new = (newfunc)[](TYPE type, OBJ args) -> OBJ
	{
		return holder_type::new_();
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DictObject::_bind_class(OBJ scope)
{
	CLASS_<DICT>(scope, "dict", get_class())

		.def(init<>())

		.def("__contains__", &DICT::contains<OBJ const &>)

		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */