#include <core/api/types/cppfunction_object.hpp>
#include <core/api/modsupport.hpp>

using namespace ism;

DECLEXPR(CoreCppFunction::ob_class) { nullptr };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static GetSetDef cppfunction_getsets[] =
{
	{ "__name__",
		(getter)[](OBJECT self, void *) -> OBJECT { return STR((***CPP_FUNCTION(self)).name); },
		(setter)[](OBJECT self, OBJECT value, void *) -> Error { return ((***CPP_FUNCTION(self)).name = ***STR(value)), Error_None; }
	},
	{ "__doc__",
		(getter)[](OBJECT self, void *) -> OBJECT { return STR((***CPP_FUNCTION(self)).doc); },
		(setter)[](OBJECT self, OBJECT value, void *) -> Error { return ((***CPP_FUNCTION(self)).doc = ***STR(value)), Error_None; }
	},
	{ /* sentinal */},
};

void CoreCppFunction::_bind_class(CoreType & t)
{
	t.tp_name = "cpp_function";
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType | TypeFlags_HaveVectorCall;

	t.tp_vectorcall_offset = offsetof(CoreCppFunction, m_vectorcall);

	t.tp_getattr = (getattrfunc)detail::impl_getattr_string;
	t.tp_setattr = (setattrfunc)detail::impl_setattr_string;
	t.tp_getattro = (getattrofunc)detail::impl_getattr_object;
	t.tp_setattro = (setattrofunc)detail::impl_setattr_object;

	t.tp_compare = (cmpfunc)[](OBJECT self, OBJECT value)
	{
		return util::compare(self.ptr(), value.ptr());
	};

	t.tp_operator_delete = (freefunc)[](void * ptr) { memdelete((CoreCppFunction *)ptr); };

	t.tp_getsets = cppfunction_getsets;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */