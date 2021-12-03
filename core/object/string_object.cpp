#include <core/object/string_object.hpp>
#include <core/object/detail/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBED_CLASS(StringObject, t)
{
	t.tp_hash = (hashfunc)[](OBJ self) -> hash_t { return ism::hash(***(STR &)self); };

	t.tp_len = (lenfunc)[](OBJ self) -> ssize_t { return (ssize_t)((STR &)self).size(); };

	t.tp_repr = (reprfunc)[](OBJ self) -> STR { return (STR &)self; };

	t.tp_str = (reprfunc)[](OBJ self) -> STR { return (STR &)self; };

	t.tp_cmp = (cmpfunc)[](OBJ self, OBJ other) -> int32_t
	{
		if (self.is(other)) {
			return 0;
		}
		else if (STR::check_(self) && STR::check_(other)) {
			return util::compare(***(STR &)self, ***(STR &)other);
		}
		else {
			return util::compare((intptr_t)*self, (intptr_t)*other);
		}
	};

	t.tp_bind = CLASS_BINDER(StringObject, t)
	{
		return t

			.def("empty", &StringObject::empty)

			.def("size", &StringObject::size)

			;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */