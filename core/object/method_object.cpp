#include <core/object/method_object.hpp>
#include <core/object/detail/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJ_IMPL(MethodObject, t, "method", TypeFlags_BaseType | TypeFlags_HaveVectorCall | TypeFlags_MethodDescriptor)
{
	t.tp_dictoffset = offsetof(MethodObject, m_dict);

	t.tp_vectorcalloffset = offsetof(MethodObject, m_vectorcall);

	t.tp_new = (newfunc)[](TYPE type, OBJ args) -> OBJ { return memnew(MethodObject); };

	t.tp_descr_get = (descrgetfunc)[](OBJ self, OBJ obj, OBJ type) { return self; };

	t.tp_bind = (bindfunc)[](TYPE type) -> TYPE
	{
		return CLASS_<MethodObject>(type);
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