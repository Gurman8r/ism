#include <core/api/object/int_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_OBJECT_IMPLEMENTATION(IntObject, t, "int", TypeFlags_Int_Subclass)
{
	t.tp_new = (newfunc)[](TYPE type, OBJ args) -> OBJ { return memnew(IntObject); };

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

	t.tp_bind = (bindfunc)[](TYPE type) -> TYPE
	{
		return CLASS_<IntObject>(type);
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

DECLEXPR(IntObject::g_true) { true };

DECLEXPR(IntObject::g_false) { false };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */