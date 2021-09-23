#include <core/object/list_object.hpp>
#include <core/object/detail/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJ_CLASS_IMPL(ListObject, t, "list", TypeFlags_List_Subclass)
{
	t.tp_new = (newfunc)[](TYPE type, OBJ args) -> OBJ { return memnew(ListObject); };

	t.tp_len = (lenfunc)[](OBJ self) { return (ssize_t)LIST(self).size(); };

	t.tp_cmp = (cmpfunc)[](OBJ self, OBJ other)
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

	t.tp_bind = (bindfunc)[](TYPE type) -> TYPE
	{
		return CLASS_<ListObject>(type)

			.def("__contains__", &ListObject::contains<OBJ const &>)

			;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */