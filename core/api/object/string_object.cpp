#include <core/api/modsupport.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

DECLEXPR(CoreString::ob_type_static) = COMPOSE(CoreType, t)
{
	t.tp_name = "str";
	t.tp_basicsize = sizeof(CoreString);
	t.tp_flags = TypeFlags_Default | TypeFlags_Str_Subclass;
	t.tp_base = typeof<OBJECT>();

	t.tp_hash = (hashfunc)[](OBJECT o) { return hash((String)STR(o)); };
	t.tp_len = (lenfunc)[](OBJECT o) { return (ssize_t)STR(o).size(); };
	t.tp_repr = (reprfunc)[](OBJECT o) { return STR(o); };
	t.tp_str = (reprfunc)[](OBJECT o) { return STR(o); };

	t.tp_alloc = (allocfunc)[](size_t size) { return memalloc(size); };
	t.tp_free = (freefunc)[](void * ptr) { memdelete((CoreString *)ptr); };

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v)
	{
		if (isinstance<STR>(v))
		{
			return util::compare((String)STR(o), (String)STR(v));
		}
		else if (auto s{ STR({ v }) })
		{
			return util::compare((String)STR(o), (String)s);
		}
		else
		{
			return util::compare(*o, *v);
		}
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void CoreString::_bind_methods(CoreType & t)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */