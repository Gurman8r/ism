#include <core/api/object/method_object.hpp>
#include <core/api/class.hpp>

using namespace ism;
using namespace ism::api;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_STATIC_CLASS_TYPE(MethodObject, t)
{
	t.tp_name = "method";
	t.tp_size = sizeof(MethodObject);
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType | TypeFlags_HaveVectorCall;
	t.tp_base = typeof<FUNCTION>();

	t.tp_vectorcall_offset = offsetof(MethodObject, m_vectorcall);

	t.tp_getattro = (getattrofunc)generic_getattr;
	t.tp_setattro = (setattrofunc)generic_setattr;

	t.tp_alloc = (allocfunc)[](size_t size) { return memalloc(size); };
	t.tp_free = (freefunc)[](void * ptr) { memdelete((MethodObject *)ptr); };

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v) { return util::compare(*o, *v); };

	t.tp_descr_get = (descrgetfunc)[](OBJECT descr, OBJECT obj, OBJECT type) { return descr; };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void MethodObject::_bind_class(TypeObject & t)
{
	CLASS_<METHOD>(&t, "method")
		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT ism::api::method_vectorcall(OBJECT callable, OBJECT const * argv, size_t argc)
{
	VERIFY(METHOD::check_(callable));
	METHOD method{ callable };
	OBJECT self{ method->m_self };
	OBJECT func{ method->m_func };
	vectorcallfunc vcall{ get_vectorcall_func(func) };

	if (argc == 0)
	{
		return vcall(func, &self, 1);
	}
	else
	{
		LIST args{ ListObject{} };
		args.reserve(1 + argc);
		args.append(self);
		for (size_t i = 0; i < argc; ++i) { args.append(argv[i]); }
		return vcall(func, args.data(), args.size());
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */