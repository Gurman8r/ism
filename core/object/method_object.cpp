#include <core/object/method_object.hpp>
#include <core/detail/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJ_IMPL(MethodObject, t, TypeFlags_BaseType | TypeFlags_HaveVectorCall | TypeFlags_MethodDescriptor)
{
	t.tp_dictoffset = offsetof(MethodObject, m_dict);

	t.tp_vectorcalloffset = offsetof(MethodObject, m_vectorcall);

	t.tp_descr_get = (descrgetfunc)[](OBJ self, OBJ obj, OBJ type) { return self; };

	t.tp_bind = CLASS_BINDER(MethodObject, c)
	{
		return c;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJ ism::method_vectorcall(OBJ callable, OBJ const * argv, size_t argc)
{
	VERIFY(METHOD::check_(callable));
	METHOD method{ callable };
	OBJ self{ method->m_self };
	OBJ func{ method->m_func };
	vectorcallfunc vcall{ get_vectorcall_func(func) };
	if (argc == 0)
	{
		return vcall(func, &self, 1);
	}
	else
	{
		LIST args{ LIST::new_(argv, argv + argc) };
		args.insert(0, self);
		return vcall(func, args.data(), args.size());
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */