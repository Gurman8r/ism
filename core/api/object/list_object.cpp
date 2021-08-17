#include <core/api/object/list_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

IMPLEMENT_CLASS(ListObject, t)
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

	t.tp_new = (newfunc)[](TYPE type, OBJ args) -> OBJ
	{
		return holder_type::new_();
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void ListObject::_bind_class(OBJ scope)
{
	CLASS_<LIST>(scope, "list", get_class())

		.def(init<>())

		.def("__contains__", &LIST::contains<OBJ const &>)

		.def("__getitem__", [](LIST self, INT index) { return self[index]; })

		.def("__setitem__", [](LIST self, INT index, OBJ value) { self[index] = value; })

		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */