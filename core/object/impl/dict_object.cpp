#include <core/object/impl/dict_object.hpp>
#include <core/object/class.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(DictObject, t)
	{
		t.tp_len = (lenfunc)[](OBJ self) -> ssize_t { return (ssize_t)DICT(self).size(); };

		t.tp_bind = BIND_CLASS(DictObject, t)
		{
			return t
				.def("__contains__", &DictObject::contains<OBJ const &>)
				;
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}