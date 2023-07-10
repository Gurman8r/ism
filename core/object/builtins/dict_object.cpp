#include <core/object/builtins/dict_object.hpp>
#include <core/object/class.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(DictObject, t)
	{
		t.tp_len = (LenFunc)[](ObjectRef self) -> ssize_t { return (ssize_t)DictRef(self).size(); };

		t.tp_bind = CLASS_(DictObject, klass)
		{
			return klass
				.def("__contains__", &DictObject::contains<ObjectRef const &>)
				;
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}