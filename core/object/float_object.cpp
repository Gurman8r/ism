#include <core/object/float_object.hpp>
#include <core/object/detail/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_IMP(FloatObject, t, TypeFlags_Float_Subclass)
{
	t.tp_new = (newfunc)[](TYPE type, OBJ args) -> OBJ { return memnew(FloatObject); };

	t.tp_hash = (hashfunc)[](OBJ self) { return Hash<double_t>()(***FLT(self)); };

	t.tp_repr = (reprfunc)[](OBJ self) { return STR(util::to_string(***FLT(self))); };

	t.tp_str = (reprfunc)[](OBJ self) { return STR(util::to_string(***FLT(self))); };

	t.tp_cmp = (cmpfunc)[](OBJ self, OBJ other)
	{
		if (FLT::check_(other))
		{
			return util::compare(***FLT(self), ***FLT(other));
		}
		else
		{
			return util::compare(*self, *other);
		}
	};

	t.tp_bind = (bindfunc)[](TYPE type) -> TYPE
	{
		return CLASS_<FloatObject>(type);
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */