#include <core/api/object/string_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_IMPLEMENT_CLASS_TYPE(StringObject, t)
{
	t.tp_name = "string";

	t.tp_size = sizeof(StringObject);

	t.tp_flags = TypeFlags_Default | TypeFlags_Str_Subclass;

	t.tp_hash = (hashfunc)[](OBJ self) { return ism::hash((String)STR(self)); };
	
	t.tp_len = (lenfunc)[](OBJ self) { return (ssize_t)STR(self).size(); };
	
	t.tp_repr = (reprfunc)[](OBJ self) { return STR(self); };
	
	t.tp_str = (reprfunc)[](OBJ self) { return STR(self); };

	t.tp_compare = (cmpfunc)[](OBJ self, OBJ other)
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

	t.tp_create = (constructor)[](TYPE type, OBJ args) -> OBJ { return memnew(StringObject); };

	t.tp_destroy = (destructor)[](Object * ptr) { memdelete((StringObject *)ptr); };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void StringObject::_bind_methods()
{
	CLASS_<STR>()

		.def(init<>())

		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */