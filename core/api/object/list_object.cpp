#include <core/api/object/list_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_OBJECT_TYPE_STATIC(ListObject, t)
{
	t.tp_flags = TypeFlags_Default | TypeFlags_List_Subclass;

	t.tp_len = (lenfunc)[](OBJECT o) { return (ssize_t)LIST(o).size(); };

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v)
	{
		if (LIST::check_(v))
		{
			return util::compare(***LIST(o), ***LIST(v));
		}
		else
		{
			return util::compare(*o, *v);
		}
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