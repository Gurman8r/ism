#include <core/api/object/int_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_OBJECT_TYPE_STATIC(IntObject, t)
{
	t.tp_flags = TypeFlags_Default | TypeFlags_Int_Subclass;

	t.tp_hash = (hashfunc)[](OBJECT o) { return Hash<int64_t>()(***INT(o)); };
	
	t.tp_repr = (reprfunc)[](OBJECT o) { return STR(util::to_string(***INT(o))); };
	
	t.tp_str = (reprfunc)[](OBJECT o) { return STR(util::to_string(***INT(o))); };

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v)
	{
		if (INT::check_(v))
		{
			return util::compare(***INT(o), ***INT(v));
		}
		else
		{
			return util::compare(*o, *v);
		}
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void IntObject::_bind_class(OBJECT scope)
{
	CLASS_<INT>(scope, "int", get_type_static())

		.def(init<>())

		.def(init<storage_type>())

		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

DECLEXPR(IntObject::g_true) { true };

DECLEXPR(IntObject::g_false) { false };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */