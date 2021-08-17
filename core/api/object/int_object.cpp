#include <core/api/object/int_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

IMPLEMENT_CLASS(IntObject, t)
{
	t.tp_flags = TypeFlags_Default | TypeFlags_Int_Subclass;

	t.tp_hash = (hashfunc)[](OBJ self) { return Hash<int64_t>()(***INT(self)); };
	
	t.tp_repr = (reprfunc)[](OBJ self) { return STR(util::to_string(***INT(self))); };
	
	t.tp_str = (reprfunc)[](OBJ self) { return STR(util::to_string(***INT(self))); };

	t.tp_compare = (cmpfunc)[](OBJ self, OBJ other)
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

	t.tp_new = (newfunc)[](TYPE type, OBJ args) -> OBJ
	{
		return holder_type::new_();
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void IntObject::_bind_class(OBJ scope)
{
	CLASS_<INT>(scope, "int", get_class())

		.def(init<>())

		.def(init<storage_type>())

		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

DECLEXPR(IntObject::g_true) { true };

DECLEXPR(IntObject::g_false) { false };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */