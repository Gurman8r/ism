#include <core/api/object/function_object.hpp>
#include <servers/script_server.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_CLASS_IMPLEMENTATION(FunctionObject, t)
{
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType | TypeFlags_HaveVectorCall | TypeFlags_MethodDescriptor;

	t.tp_dictoffset = offsetof(FunctionObject, m_dict);

	t.tp_vectorcalloffset = offsetof(FunctionObject, m_vectorcall);

	t.tp_compare = (cmpfunc)[](OBJ self, OBJ other) { return util::compare(*self, *other); };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FunctionObject::_bind_class(OBJ scope)
{
	CLASS_<FUNCTION>(scope, "function")

		.def(init<>())

		//.def_property("__name__", [](FUNCTION self) { return self->m_name; }, [](FUNCTION self, STR value) { self->m_name = value; })

		//.def_property("__doc__", [](FUNCTION self) { return self->m_doc; }, [](FUNCTION self, STR value) { self->m_doc = value; })

		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */