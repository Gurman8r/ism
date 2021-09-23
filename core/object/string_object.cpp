#include <core/object/string_object.hpp>
#include <core/object/detail/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJ_CLASS_IMPL(StringObject, t, "string", TypeFlags_Str_Subclass)
{
	t.tp_new = (newfunc)[](TYPE type, OBJ args) -> OBJ { return memnew(StringObject); };

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

	t.tp_bind = (bindfunc)[](TYPE type) -> TYPE
	{
		return CLASS_<StringObject>(type)
			
			.def("empty", &StringObject::empty)

			.def("size", &StringObject::size)

			;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */