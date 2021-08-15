#include <core/api/object/method_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_COMPOSE_TYPE_OBJECT(MethodObject, t)
{
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType | TypeFlags_HaveVectorCall | TypeFlags_MethodDescriptor;

	t.tp_dictoffset = offsetof(MethodObject, m_dict);

	t.tp_vectorcalloffset = offsetof(MethodObject, m_vectorcall);

	t.tp_compare = (cmpfunc)[](OBJECT self, OBJECT other) { return util::compare(*self, *other); };

	t.tp_descr_get = (descrgetfunc)[](OBJECT self, OBJECT obj, OBJECT type) { return self; };

	t.tp_new = (newfunc)[](TYPE type, OBJECT args) -> OBJECT
	{
		return holder_type::new_();
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void MethodObject::_bind_class(OBJECT scope)
{
	CLASS_<METHOD>(scope, "method", get_type_static())

		//.def(init<>())

		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT ism::method_vectorcall(OBJECT callable, OBJECT const * argv, size_t argc)
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
		LIST args{ LIST::new_() };
		args.reserve(1 + argc);
		args.append(self);
		for (size_t i = 0; i < argc; ++i) { args.append(argv[i]); }
		return vcall(func, args.data(), args.size());
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */