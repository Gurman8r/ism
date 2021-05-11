#include <core/api/types/cppfunction_object.hpp>
#include <core/api/modsupport.hpp>

using namespace ism;

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

DECLEXPR(CoreCppFunction::ob_type_static) = COMPOSE(CoreType, t)
{
	t.tp_name = "cpp_function";
	t.tp_basicsize = sizeof(CoreCppFunction);
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType | TypeFlags_HaveVectorCall;
	t.tp_base = typeof<FUNCTION>();

	t.tp_vectorcall_offset = offsetof(CoreCppFunction, m_vectorcall);

	t.tp_getattr = (getattrfunc)detail::impl_getattr_string;
	t.tp_setattr = (setattrfunc)detail::impl_setattr_string;
	t.tp_getattro = (getattrofunc)detail::impl_getattr_object;
	t.tp_setattro = (setattrofunc)detail::impl_setattr_object;

	t.tp_compare = (cmpfunc)[](OBJECT self, OBJECT value)
	{
		return util::compare(self.ptr(), value.ptr());
	};

	t.tp_alloc = (allocfunc)[](size_t size) { return memalloc(size); };
	t.tp_free = (freefunc)[](void * ptr) { memdelete((CoreCppFunction *)ptr); };

	t.tp_getsets = cppfunction_getsets;
};

void CoreCppFunction::_bind_class(CoreType & t)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */