#include <core/api/object/method_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

IMPLEMENT_CLASS(MethodObject, t)
{
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType | TypeFlags_HaveVectorCall | TypeFlags_MethodDescriptor;

	t.tp_dictoffset = offsetof(MethodObject, m_dict);

	t.tp_vectorcalloffset = offsetof(MethodObject, m_vectorcall);

	t.tp_compare = (cmpfunc)[](OBJ self, OBJ other) { return CMP(*self, *other); };

	t.tp_descr_get = (descrgetfunc)[](OBJ self, OBJ obj, OBJ type) { return self; };

	t.tp_new = (newfunc)[](TYPE type, OBJ args) -> OBJ
	{
		return holder_type::new_();
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void MethodObject::_bind_class(OBJ scope)
{
	CLASS_<METHOD>(scope, "method", get_class())

		//.def(init<>())

		;
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
		LIST args{ LIST::new_() };
		args.reserve(1 + argc);
		args.append(self);
		for (size_t i = 0; i < argc; ++i) { args.append(argv[i]); }
		return vcall(func, args.data(), args.size());
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */