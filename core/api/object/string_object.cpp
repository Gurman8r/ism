#include <core/api/object/string_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

IMPLEMENT_CLASS(StringObject, t)
{
	t.tp_flags = TypeFlags_Default | TypeFlags_Str_Subclass;
	
	t.tp_hash = (hashfunc)[](OBJ self) { return ism::hash((String)STR(self)); };
	
	t.tp_len = (lenfunc)[](OBJ self) { return (ssize_t)STR(self).size(); };
	
	t.tp_repr = (reprfunc)[](OBJ self) { return STR(self); };
	
	t.tp_str = (reprfunc)[](OBJ self) { return STR(self); };

	t.tp_compare = (cmpfunc)[](OBJ self, OBJ other)
	{
		if (holder_type::check_(other))
		{
			return util::compare((String)STR(self), (String)STR(other));
		}
		else if (holder_type s{ { other } })
		{
			return util::compare((String)STR(self), (String)s);
		}
		else
		{
			return util::compare(*self, *other);
		}
	};

	t.tp_new = (newfunc)[](TYPE type, OBJ args) -> OBJ
	{
		return holder_type::new_();
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void StringObject::_bind_class(OBJ scope)
{
	CLASS_<STR>(scope, "string", get_class())
		.def(init<>())
		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */