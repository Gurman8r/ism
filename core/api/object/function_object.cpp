#include <core/api/object/function_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_OBJECT_TYPE_STATIC(FunctionObject, t)
{
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType | TypeFlags_HaveVectorCall | TypeFlags_MethodDescriptor;

	t.tp_dictoffset = offsetof(FunctionObject, m_dict);

	t.tp_vectorcalloffset = offsetof(FunctionObject, m_vectorcall);

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v) { return util::compare(*o, *v); };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FunctionObject::_bind_class(OBJECT scope)
{
	CLASS_<FUNCTION>(scope, "function", get_type_static())

		//.def(init<>())

		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */