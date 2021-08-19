#include <core/api/object/list_object.hpp>
#include <servers/script_server.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_CLASS_IMPLEMENTATION(ListObject, t)
{
	t.tp_flags = TypeFlags_Default | TypeFlags_List_Subclass;

	t.tp_len = (lenfunc)[](OBJ self) { return (ssize_t)LIST(self).size(); };

	t.tp_compare = (cmpfunc)[](OBJ self, OBJ other)
	{
		if (LIST::check_(other))
		{
			return util::compare(***LIST(self), ***LIST(other));
		}
		else
		{
			return util::compare(*self, *other);
		}
	};

	t.tp_vectorcall = (vectorcallfunc)[](OBJ callable, OBJ const * argv, size_t argc) -> OBJ
	{
		return nullptr;
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void ListObject::_bind_class(OBJ scope)
{
	CLASS_<LIST>(scope, "list")

		.def(init<>())

		.def("__contains__", &LIST::contains<OBJ const &>)

		.def("__getitem__", [](LIST self, INT index) { return self[index]; })

		.def("__setitem__", [](LIST self, INT index, OBJ value) { self[index] = value; })

		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */