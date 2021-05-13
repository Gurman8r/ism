#include <core/api/modsupport.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

DECLEXPR(CoreFloat::ob_type_static) = COMPOSE(CoreType, t)
{
	t.tp_name = "float";
	t.tp_basicsize = sizeof(CoreFloat);
	t.tp_flags = TypeFlags_Default | TypeFlags_Float_Subclass;
	t.tp_base = typeof<OBJECT>();

	t.tp_hash = (hashfunc)[](OBJECT o) { return Hash<double_t>()(***FLT(o)); };
	t.tp_repr = (reprfunc)[](OBJECT o) { return STR(util::to_string(***FLT(o))); };
	t.tp_str = (reprfunc)[](OBJECT o) { return STR(util::to_string(***FLT(o))); };

	t.tp_alloc = (allocfunc)[](size_t size) { return memalloc(size); };
	t.tp_free = (freefunc)[](void * ptr) { memdelete((CoreFloat *)ptr); };

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v)
	{
		if (isinstance<FLT>(v)) return util::compare(***FLT(o), ***FLT(v));
		return util::compare(o.ptr(), v.ptr());
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void CoreFloat::_bind_methods(CoreType & t)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */