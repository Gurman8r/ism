#include <core/object/list_object.hpp>
#include <core/object/detail/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_EMBED(ListObject, t)
{
	t.tp_len = (lenfunc)[](OBJ self) -> ssize_t { return (ssize_t)LIST(self).size(); };

	t.tp_cmp = (cmpfunc)[](OBJ self, OBJ other) -> int32_t
	{
		if (self.is(other)) { return 0; }
		
		else if (LIST::check_(self) && LIST::check_(other)) { return util::compare(***(LIST &)self, ***(LIST &)other); }
		
		else { return util::compare((intptr_t)*self, (intptr_t)*other); }
	};

	t.tp_install = CLASS_INSTALLER(ListObject, t)
	{
		return t

			.def("__contains__", &ListObject::contains<OBJ const &>)

			;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */