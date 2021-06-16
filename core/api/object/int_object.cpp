#include <core/api/object/int_object.hpp>
#include <core/api/object/generic_object.hpp>

using namespace ism;
using namespace ism::api;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_BUILTIN_TYPE(IntObject, t)
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
		if (isinstance<INT>(v))
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

void IntObject::_bind_methods(TypeObject & t)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

DECLEXPR(IntObject::g_true) { true };

DECLEXPR(IntObject::g_false) { false };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */