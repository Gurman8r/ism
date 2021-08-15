#include <core/api/object/function_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

IMPLEMENT_CLASS_TYPE(FunctionObject, t)
{
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType | TypeFlags_HaveVectorCall | TypeFlags_MethodDescriptor;

	t.tp_dictoffset = offsetof(FunctionObject, m_dict);

	t.tp_vectorcalloffset = offsetof(FunctionObject, m_vectorcall);

	t.tp_compare = (cmpfunc)[](OBJECT self, OBJECT other) { return util::compare(*self, *other); };

	t.tp_new = (newfunc)[](TYPE type, OBJECT args) -> OBJECT
	{
		return holder_type::new_();
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FunctionObject::_bind_class(OBJECT scope)
{
	CLASS_<FUNCTION>(scope, "function", get_type_static())

		//.def(init<>())

		//.def_property("__name__", [](FUNCTION self) { return self->m_name; }, [](FUNCTION self, STR value) { self->m_name = value; })

		//.def_property("__doc__", [](FUNCTION self) { return self->m_doc; }, [](FUNCTION self, STR value) { self->m_doc = value; })

		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */