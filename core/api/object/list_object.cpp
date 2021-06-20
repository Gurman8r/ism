#include <core/api/object/list_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_OBJECT_TYPE_STATIC(ListObject, t)
{
	t.tp_name = "list";
	t.tp_size = sizeof(ListObject);
	t.tp_flags = TypeFlags_Default | TypeFlags_List_Subclass;
	t.tp_base = typeof<OBJECT>();

	t.tp_len = (lenfunc)[](OBJECT o) { return (ssize_t)LIST(o).size(); };

	t.tp_alloc = (allocfunc)[](size_t size) { return memalloc(size); };
	t.tp_free = (freefunc)[](void * ptr) { memdelete((ListObject *)ptr); };

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

void ListObject::_bind_class(TypeObject & t)
{
	CLASS_<LIST>(&t, "list")

		//.def(init<>())

		.def("__contains__", [](LIST self, OBJECT value) { return LIST(self).contains(value); })

		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */