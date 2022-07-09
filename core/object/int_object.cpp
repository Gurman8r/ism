#include <core/object/int_object.hpp>
#include <core/object/detail/class.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(IntObject, t)
	{
		t.tp_hash = (hashfunc)[](OBJ self) -> hash_t { return Hasher<int64_t>()(***(INT &)self); };

		t.tp_repr = (reprfunc)[](OBJ self) -> STR { return STR(util::to_string(***(INT &)self)); };

		t.tp_str = (reprfunc)[](OBJ self) -> STR { return STR(util::to_string(***(INT &)self)); };

		t.tp_cmp = (cmpfunc)[](OBJ self, OBJ other) -> int32_t
		{
			if (self.is(other)) { return 0; }

			else if (INT::check_(self) && INT::check_(other)) { return util::compare(***(INT &)self, ***(INT &)other); }

			else { return util::compare((intptr_t)*self, (intptr_t)*other); }
		};

		t.tp_install = CLASS_INSTALLER(IntObject, t)
		{
			return t;
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	IntObject IntObject::g_true{ true };

	IntObject IntObject::g_false{ false };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}