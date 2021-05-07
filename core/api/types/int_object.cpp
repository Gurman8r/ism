#include <core/api/types/int_object.hpp>
#include <core/api/modsupport.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

DECLEXPR(CoreInt::ob_class) { nullptr };

CoreInt CoreInt::g_true = CoreInt{ true };

CoreInt CoreInt::g_false = CoreInt{ false };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void CoreInt::_bind_class(CoreType & t)
{
	t.tp_name = "int";
	t.tp_basicsize = sizeof(CoreInt);
	t.tp_flags = TypeFlags_Default | TypeFlags_Int_Subclass;

	t.tp_hash = (hashfunc)[](OBJECT o) { return Hash<int64_t>()(***INT(o)); };
	t.tp_repr = (reprfunc)[](OBJECT o) { return STR(util::to_string(***INT(o))); };
	t.tp_str = (reprfunc)[](OBJECT o) { return STR(util::to_string(***INT(o))); };

	t.tp_operator_delete = (freefunc)[](void * ptr) { memdelete((CoreInt *)ptr); };

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v)
	{
		if (isinstance<INT>(v)) return util::compare(***INT(o), ***INT(v));
		return util::compare(o.ptr(), v.ptr());
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */