#include <core/object/string_object.hpp>
#include <core/object/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBED_CLASS(StringObject, t, TypeFlags_Str_Subclass)
{
	t.tp_hash = (hashfunc)[](OBJ self) { return ism::hash((String)STR(self)); };

	t.tp_len = (lenfunc)[](OBJ self) { return (ssize_t)STR(self).size(); };

	t.tp_repr = (reprfunc)[](OBJ self) { return STR(self); };

	t.tp_str = (reprfunc)[](OBJ self) { return STR(self); };

	t.tp_cmp = (cmpfunc)[](OBJ self, OBJ other)
	{
		if (STR::check_(other))
		{
			return util::compare((String)STR(self), (String)STR(other));
		}
		else if (STR s{ { other } })
		{
			return util::compare((String)STR(self), (String)s);
		}
		else
		{
			return util::compare(*self, *other);
		}
	};

	t.tp_bind = CLASS_BINDFUNC(StringObject, t)
	{
		return t

			.def("empty", &StringObject::empty)

			.def("size", &StringObject::size)

			;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */