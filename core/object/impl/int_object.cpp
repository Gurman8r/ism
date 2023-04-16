#include <core/object/impl/int_object.hpp>
#include <core/object/class.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(IntObject, t)
	{
		t.tp_hash = (hashfunc)[](OBJ self) -> size_t { return Hasher<i64>()(***(INT &)self); };

		t.tp_repr = (reprfunc)[](OBJ self) -> STR { return STR(util::to_string(***(INT &)self)); };

		t.tp_str = (reprfunc)[](OBJ self) -> STR { return STR(util::to_string(***(INT &)self)); };

		t.tp_cmp = (cmpfunc)[](OBJ self, OBJ other) -> i32
		{
			if (self.is(other)) { return 0; }

			else if (INT::check_(self) && INT::check_(other)) { return compare(***(INT &)self, ***(INT &)other); }

			else { return compare((intptr_t)*self, (intptr_t)*other); }
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	IntObject IntObject::g_True{ true };

	IntObject IntObject::g_False{ false };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}