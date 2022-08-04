#include <core/object/iterator_object.hpp>
#include <core/object/detail/class.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_OBJECT_CLASS(IteratorObject, t)
	{
		t.tp_cmp = (cmpfunc)[](OBJ self, OBJ other) -> i32
		{
			return util::compare(*self, *other);
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}