#include <core/api/object/int_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_OBJECT_TYPE_STATIC(IntObject, t)
{
	t.tp_name = "int";
	t.tp_size = sizeof(IntObject);
	t.tp_flags = TypeFlags_Default | TypeFlags_Int_Subclass;
	t.tp_base = typeof<OBJECT>();

	t.tp_hash = (hashfunc)[](OBJECT o) { return Hash<int64_t>()(***INT(o)); };
	t.tp_repr = (reprfunc)[](OBJECT o) { return STR(util::to_string(***INT(o))); };
	t.tp_str = (reprfunc)[](OBJECT o) { return STR(util::to_string(***INT(o))); };

	t.tp_alloc = (allocfunc)[](size_t size) { return memalloc(size); };
	t.tp_free = (freefunc)[](void * ptr) { memdelete((IntObject *)ptr); };

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

void IntObject::_bind_class(TypeObject & t)
{
	CLASS_<INT>(&t, "int")
		//.def(init<>())
		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

DECLEXPR(IntObject::g_true) { true };

DECLEXPR(IntObject::g_false) { false };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */