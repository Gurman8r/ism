#include <core/api/object/float_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBEDDED_CLASS_TYPE(FloatObject, t, TypeFlags_Float_Subclass)
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