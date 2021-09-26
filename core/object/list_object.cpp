#include <core/object/list_object.hpp>
#include <core/object/detail/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJ_IMPL(ListObject, t, TypeFlags_List_Subclass)
{
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

	t.tp_bind = CLASS_BINDER(ListObject, t)
	{
		return t

			.def("__contains__", &ListObject::contains<OBJ const &>)

			;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */