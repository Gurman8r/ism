#include <core/object/float_object.hpp>
#include <core/detail/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJ_IMPL(FloatObject, t, TypeFlags_Float_Subclass)
{
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

	t.tp_bind = CLASS_BINDER(FloatObject, c)
	{
		return c;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */