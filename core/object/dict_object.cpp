#include <core/object/dict_object.hpp>
#include <core/object/detail/class.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(DictObject, t)
	{
		t.tp_len = (lenfunc)[](OBJ self) -> ssize_t { return (ssize_t)DICT(self).size(); };

		t.tp_install = CLASS_INSTALLER(DictObject, t)
		{
			return t

				.def("__contains__", &DictObject::contains<OBJ const &>)

				;
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}