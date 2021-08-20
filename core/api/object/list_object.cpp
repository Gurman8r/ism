#include <core/api/object/list_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_IMPLEMENT_CLASS_TYPE(ListObject, t)
{
	t.tp_name = "list";

	t.tp_size = sizeof(ListObject);

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

	t.tp_create = (constructor)[](TYPE type, OBJ args) -> OBJ { return memnew(ListObject); };

	t.tp_destroy = (destructor)[](Object * ptr) { memdelete((ListObject *)ptr); };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void ListObject::_bind_methods()
{
	CLASS_<LIST>()

		.def(init<>())

		.def("__contains__", &LIST::contains<OBJ const &>)

		.def("__getitem__", [](LIST self, INT index) { return self[index]; })

		.def("__setitem__", [](LIST self, INT index, OBJ value) { self[index] = value; })

		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */