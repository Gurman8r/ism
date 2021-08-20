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

	t.tp_create = (constructor)[](TYPE type, OBJ args) -> OBJ { return memnew(FunctionObject); };

	t.tp_destroy = (destructor)[](Object * ptr) { memdelete((FunctionObject *)ptr); };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FunctionObject::_bind_methods()
{
	CLASS_<FUNCTION>()

		.def(init<>())

		//.def_property("__name__", [](FUNCTION self) { return self->m_name; }, [](FUNCTION self, STR value) { self->m_name = value; })

		//.def_property("__doc__", [](FUNCTION self) { return self->m_doc; }, [](FUNCTION self, STR value) { self->m_doc = value; })

		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */