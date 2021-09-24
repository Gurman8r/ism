#include <core/object/dict_object.hpp>
#include <core/detail/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJ_IMPL(DictObject, t, TypeFlags_Dict_Subclass)
{
	t.tp_len = (lenfunc)[](OBJ self) { return (ssize_t)DICT(self).size(); };

	t.tp_bind = CLASS_BINDER(DictObject, c)
	{
		return c

			.def("__contains__", &DictObject::contains<OBJ const &>)

			;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */