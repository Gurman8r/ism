#include <core/api/object/function_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_OBJECT_IMPLEMENTATION(FunctionObject, t, "function", TypeFlags_BaseType | TypeFlags_HaveVectorCall | TypeFlags_MethodDescriptor)
{
	t.tp_dictoffset = offsetof(FunctionObject, m_dict);

	t.tp_vectorcalloffset = offsetof(FunctionObject, m_vectorcall);

	t.tp_new = (newfunc)[](TYPE type, OBJ args) -> OBJ { return memnew(FunctionObject); };

	t.tp_bind = (bindfunc)[](TYPE type) -> TYPE
	{
		return CLASS_<FUNCTION>(type)

			.def_property_readonly("__dict__", [](FUNCTION self) { return self->m_dict; })

			.def_property_readonly("__module__", [](FUNCTION self) { return self->m_module; })

			;
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */