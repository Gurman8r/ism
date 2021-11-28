#include <core/api/object/int_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBED_CLASS(IntObject, t)
{
	t.tp_hash = (hashfunc)[](OBJ self) -> hash_t { return Hasher<int64_t>()(***(INT &)self); };

	t.tp_repr = (reprfunc)[](OBJ self) -> STR { return STR(util::to_string(***(INT &)self)); };

	t.tp_str = (reprfunc)[](OBJ self) -> STR { return STR(util::to_string(***(INT &)self)); };

	t.tp_cmp = (cmpfunc)[](OBJ self, OBJ other) -> int32_t
	{
		if (self.is(other)) {
			return 0;
		}
		else if (INT::check_(self) && INT::check_(other)) {
			return util::compare(***(INT &)self, ***(INT &)other);
		}
		else {
			return util::compare((intptr_t)*self, (intptr_t)*other);
		}
	};

	CLASS_DEFINITION(IntObject, t)
	{
		return t;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

MEMBER_IMPL(IntObject::g_true) { true };

MEMBER_IMPL(IntObject::g_false) { false };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */