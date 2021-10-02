#include <core/api/object/list_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBEDDED_CLASS_TYPE(ListObject, t, TypeFlags_List_Subclass)
{
	t.tp_len = (lenfunc)[](OBJ self) -> ssize_t { return (ssize_t)LIST(self).size(); };

	t.tp_cmp = (cmpfunc)[](OBJ self, OBJ other) -> int32_t
	{
		if (self.is(other)) {
			return 0;
		}
		else if (LIST::check_(self) && LIST::check_(other)) {
			return util::compare(***(LIST &)self, ***(LIST &)other);
		}
		else {
			return util::compare((intptr_t)*self, (intptr_t)*other);
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