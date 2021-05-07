#include <core/api/types/function_object.hpp>
#include <core/api/modsupport.hpp>

using namespace ism;

DECLEXPR(CoreFunction::ob_class) { nullptr };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static GetSetDef function_getsets[] =
{
	{ /* sentinal */ },
};

void CoreFunction::_bind_class(CoreType & t)
{
	t.tp_name = "function";
	t.tp_basicsize = sizeof(CoreFunction);
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType | TypeFlags_HaveVectorCall;

	t.tp_vectorcall_offset = offsetof(CoreFunction, m_vectorcall);

	t.tp_getattr = (getattrfunc)detail::impl_getattr_string;
	t.tp_setattr = (setattrfunc)detail::impl_setattr_string;
	t.tp_getattro = (getattrofunc)detail::impl_getattr_object;
	t.tp_setattro = (setattrofunc)detail::impl_setattr_object;

	t.tp_operator_delete = (freefunc)[](void * ptr) { memdelete((CoreFunction *)ptr); };

	t.tp_getsets = function_getsets;

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v)
	{
		return util::compare(o.ptr(), v.ptr());
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */