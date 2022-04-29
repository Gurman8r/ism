#include <core/object/float_object.hpp>
#include <core/object/detail/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBED_OBJECT_CLASS(FloatObject, t)
{
	t.tp_hash = (hashfunc)[](OBJ self) -> hash_t { return Hasher<double_t>()(***(FLT &)self); };

	t.tp_repr = (reprfunc)[](OBJ self) -> STR { return STR(util::to_string(***(FLT &)self)); };

	t.tp_str = (reprfunc)[](OBJ self) -> STR { return STR(util::to_string(***(FLT &)self)); };

	t.tp_cmp = (cmpfunc)[](OBJ self, OBJ other) -> int32_t
	{
		if (self.is(other)) {
			return 0;
		}
		else if (FLT::check_(self) && FLT::check_(other)) {
			return util::compare(***(FLT &)self, ***(FLT &)other);
		}
		else {
			return util::compare((intptr_t)*self, (intptr_t)*other);
		}
	};

	t.tp_bind = CLASS_BINDER(FloatObject, t)
	{
		return t;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */