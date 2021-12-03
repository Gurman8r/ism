#include <core/object/dict_object.hpp>
#include <core/object/detail/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBED_CLASS(DictObject, t)
{
	t.tp_len = (lenfunc)[](OBJ self) -> ssize_t { return (ssize_t)DICT(self).size(); };

	t.tp_bind = CLASS_BINDER(DictObject, t)
	{
		return t

			.def("__contains__", &DictObject::contains<OBJ const &>)

			;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */