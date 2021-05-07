#include <core/api/types/string_object.hpp>
#include <core/api/modsupport.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

DECLEXPR(CoreString::ob_class) { nullptr };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void CoreString::_bind_class(CoreType & t)
{
	t.tp_name = "str";
	t.tp_basicsize = sizeof(CoreString);
	t.tp_flags = TypeFlags_Default | TypeFlags_Str_Subclass;

	t.tp_hash = (hashfunc)[](OBJECT o) { return hash(***STR(o)); };
	t.tp_len = (lenfunc)[](OBJECT o) { return (ssize_t)STR(o)->size(); };
	t.tp_repr = (reprfunc)[](OBJECT o) { return STR(o); };
	t.tp_str = (reprfunc)[](OBJECT o) { return STR(o); };

	t.tp_operator_delete = (freefunc)[](void * ptr) { memdelete((CoreString *)ptr); };

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v)
	{
		if (isinstance<STR>(v)) return util::compare(***STR(o), ***STR(v));
		else if (auto s{ STR({ v }) }) return util::compare(***STR(o), ***s);
		return util::compare(o.ptr(), v.ptr());
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */