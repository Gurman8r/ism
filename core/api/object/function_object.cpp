#include <core/api/object/function_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_IMPLEMENT_CLASS_TYPE(FunctionObject, t)
{
	t.tp_name = "function";

	t.tp_size = sizeof(FunctionObject);

	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType | TypeFlags_HaveVectorCall | TypeFlags_MethodDescriptor;

	t.tp_dictoffset = offsetof(FunctionObject, m_dict);

	t.tp_vectorcalloffset = offsetof(FunctionObject, m_vectorcall);

	t.tp_new = (newfunc)[](TYPE type, OBJ args) -> OBJ { return memnew(FunctionObject); };

	t.tp_del = (delfunc)[](Object * ptr) { memdelete((FunctionObject *)ptr); };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FunctionObject::_bind_methods()
{
	CLASS_<FUNCTION>()

		//.def_property("__name__", [](FUNCTION self) { return self->m_name; }, [](FUNCTION self, STR value) { self->m_name = value; })

		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */