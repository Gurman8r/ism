#include <core/api/object/list_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

IMPLEMENT_CLASS_TYPE(ListObject, t)
{
	t.tp_flags = TypeFlags_Default | TypeFlags_List_Subclass;

	t.tp_len = (lenfunc)[](OBJECT self) { return (ssize_t)LIST(self).size(); };

	t.tp_compare = (cmpfunc)[](OBJECT self, OBJECT other)
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

	t.tp_new = (newfunc)[](TYPE type, OBJECT args) -> OBJECT
	{
		return holder_type::new_();
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void ListObject::_bind_class(OBJECT scope)
{
	CLASS_<LIST>(scope, "list", get_type_static())

		.def(init<>())

		.def("__contains__", &LIST::contains<OBJECT const &>)

		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */