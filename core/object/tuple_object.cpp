#include <core/object/tuple_object.hpp>
#include <core/object/detail/class.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(TupleObject, t)
	{
		t.tp_cmp = (cmpfunc)[](OBJ self, OBJ other) -> int32_t
		{
			return util::compare(*self, *other);
		};

		t.tp_install = CLASS_INSTALLER(TupleObject, t)
		{
			return t;
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}