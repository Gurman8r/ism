#include <core/api/object/method_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBEDDED_CLASS_TYPE(MethodObject, t, TypeFlags_HaveVectorCall | TypeFlags_MethodDescriptor)
{
	t.tp_dictoffset = offsetof(MethodObject, m_dict);

	t.tp_vectorcalloffset = offsetof(MethodObject, m_vectorcall);

	t.tp_descr_get = (descrgetfunc)[](OBJ self, OBJ obj, OBJ cls) { return self; };

	t.tp_bind = CLASS_BINDER(MethodObject, t)
	{
		return t;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJ MethodObject::method_vectorcall(OBJ callable, OBJ const * argv, size_t argc)
{
	VERIFY(METHOD::check_(callable));
	OBJ & self{ ((METHOD &)callable)->m_self };
	OBJ & func{ ((METHOD &)callable)->m_func };
	vectorcallfunc vcall{ get_vectorcall_func(func) };
	if (argc == 0)
	{
		return vcall(func, &self, 1);
	}
	else
	{
		VERIFY((argc + 1) < MAX_ARGUMENTS);
		
		OBJ stack[MAX_ARGUMENTS]{};

		stack[0] = self;
		
		std::copy(argv, argv + 1, stack + 1);
		
		return vcall(func, stack, argc + 1);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */