#include <core/object/dict_object.hpp>
#include <core/object/detail/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJ_IMPL(DictObject, t, "dict", TypeFlags_Dict_Subclass)
{
	t.tp_new = (newfunc)[](TYPE type, OBJ args) -> OBJ { return memnew(DictObject); };

	t.tp_len = (lenfunc)[](OBJ self) { return (ssize_t)DICT(self).size(); };

	t.tp_bind = (bindfunc)[](TYPE type) -> TYPE
	{
		return CLASS_<DictObject>(type)

			.def("__contains__", &DictObject::contains<OBJ const &>)

			;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */