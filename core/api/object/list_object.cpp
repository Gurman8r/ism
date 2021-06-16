#include <core/api/object/list_object.hpp>
#include <core/api/object/generic_object.hpp>

using namespace ism;
using namespace ism::api;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_BUILTIN_TYPE(ListObject, t)
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
		if (isinstance<LIST>(v))
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

void ListObject::_bind_methods(TypeObject & t)
{
	t.tp_dict["__contains__"] = CPP_FUNCTION([](OBJECT self, OBJECT value) -> OBJECT {
		return Core_Bool(LIST(self)->contains(value));
	});
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */