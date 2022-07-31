#include <core/object/list_object.hpp>
#include <core/object/detail/class.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(ListObject, t)
	{
		t.tp_len = (lenfunc)[](OBJ self) -> ssize_t { return (ssize_t)LIST(self).size(); };

		t.tp_cmp = (cmpfunc)[](OBJ self, OBJ other) -> i32
		{
			if (self.is(other)) { return 0; }

			else if (LIST::check_(self) && LIST::check_(other)) { return util::compare(***(LIST &)self, ***(LIST &)other); }

			else { return util::compare((intptr_t)*self, (intptr_t)*other); }
		};

		t.tp_bind = CLASS_INSTALLER(ListObject, t)
		{
			return t
				.def("__contains__", &ListObject::contains<OBJ const &>)
				;
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}