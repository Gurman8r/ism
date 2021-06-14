#include <core/api/object/cppfunction_object.hpp>
#include <core/api/modsupport.hpp>

using namespace ism;
using namespace ism::api;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static GetSetDef cppfunction_getsets[] =
{
	{ "__name__",
		(getter)[](OBJECT self, void *) -> OBJECT { return STR((***CPP_FUNCTION(self)).name); },
		(setter)[](OBJECT self, OBJECT value, void *) -> Error { return ((***CPP_FUNCTION(self)).name = (String)STR(value)), Error_None; }
	},
	{ "__doc__",
		(getter)[](OBJECT self, void *) -> OBJECT { return STR((***CPP_FUNCTION(self)).doc); },
		(setter)[](OBJECT self, OBJECT value, void *) -> Error { return ((***CPP_FUNCTION(self)).doc = (String)STR(value)), Error_None; }
	},
	{ /* sentinal */},
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

STATIC_MEMBER(CppFunctionObject::ob_type_static) = COMPOSE(TypeObject, t)
{
	t.tp_name = "cpp_function";
	t.tp_basicsize = sizeof(CppFunctionObject);
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType | TypeFlags_HaveVectorCall;
	t.tp_base = typeof<FUNCTION>();

	t.tp_vectorcall_offset = offsetof(CppFunctionObject, m_vectorcall);

	t.tp_getattr = (getattrfunc)impl_getattr_string;
	t.tp_setattr = (setattrfunc)impl_setattr_string;
	t.tp_getattro = (getattrofunc)impl_getattr_object;
	t.tp_setattro = (setattrofunc)impl_setattr_object;

	t.tp_alloc = (allocfunc)[](size_t size) { return memalloc(size); };
	t.tp_free = (freefunc)[](void * ptr) { memdelete((CppFunctionObject *)ptr); };

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v) { return util::compare(*o, *v); };

	t.tp_getsets = cppfunction_getsets;
};

void CppFunctionObject::_bind_class(TypeObject & t)
{
	//t.attr("__name__") = PROPERTY({
	//	CPP_FUNCTION([](CPP_FUNCTION self) { return self->m_cppfunction.name; }),
	//	CPP_FUNCTION([](CPP_FUNCTION self, STR value) { self->m_cppfunction.name = value; })
	//});
	//
	//t.attr("__doc__") = PROPERTY({
	//	CPP_FUNCTION([](CPP_FUNCTION self) { return self->m_cppfunction.doc; }),
	//	CPP_FUNCTION([](CPP_FUNCTION self, STR value) { self->m_cppfunction.doc = value; })
	//});
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */