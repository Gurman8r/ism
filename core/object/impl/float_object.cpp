#include <core/object/impl/float_object.hpp>
#include <core/object/class.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(FloatObject, t)
	{
		t.tp_hash = (hashfunc)[](OBJ self) -> size_t { return Hasher<f64>()(***(FLT &)self); };

		t.tp_repr = (reprfunc)[](OBJ self) -> STR { return STR(util::to_string(***(FLT &)self)); };

		t.tp_str = (reprfunc)[](OBJ self) -> STR { return STR(util::to_string(***(FLT &)self)); };

		t.tp_cmp = (cmpfunc)[](OBJ self, OBJ other) -> i32
		{
			if (self.is(other)) { return 0; }

			else if (FLT::check_(self) && FLT::check_(other)) { return compare(***(FLT &)self, ***(FLT &)other); }

			else { return compare((intptr_t)*self, (intptr_t)*other); }
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}