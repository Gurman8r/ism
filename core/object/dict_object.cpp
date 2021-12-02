#include <core/object/dict_object.hpp>
#include <core/object/detail/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBEDED_CLASS(DictObject, t)
{
	t.tp_len = (lenfunc)[](OBJ self) -> ssize_t { return (ssize_t)DICT(self).size(); };

	CLASS_DEF(DictObject, t)
	{
		return t

			.def("__contains__", &DictObject::contains<OBJ const &>)

			;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */