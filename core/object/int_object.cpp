#include <core/object/int_object.hpp>
#include <core/detail/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJ_IMPL(IntObject, t, TypeFlags_Int_Subclass)
{
	t.tp_hash = (hashfunc)[](OBJ self) { return Hash<int64_t>()(***INT(self)); };

	t.tp_repr = (reprfunc)[](OBJ self) { return STR(util::to_string(***INT(self))); };

	t.tp_str = (reprfunc)[](OBJ self) { return STR(util::to_string(***INT(self))); };

	t.tp_cmp = (cmpfunc)[](OBJ self, OBJ other)
	{
		if (INT::check_(other))
		{
			return util::compare(***INT(self), ***INT(other));
		}
		else
		{
			return util::compare(*self, *other);
		}
	};

	t.tp_bind = CLASS_BINDER(IntObject, c)
	{
		return c;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

VAR_IMPL(IntObject::g_true) { true };

VAR_IMPL(IntObject::g_false) { false };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */