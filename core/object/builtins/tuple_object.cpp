#include <core/object/builtins/tuple_object.hpp>
#include <core/object/class.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(TupleObject, t)
	{
		t.tp_cmp = (CmpFunc)[](ObjectRef self, ObjectRef other) -> i32
		{
			return compare(*self, *other);
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}