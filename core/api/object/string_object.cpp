#include <core/api/object/string_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_COMPOSE_TYPE_OBJECT(StringObject, t)
{
	t.tp_flags = TypeFlags_Default | TypeFlags_Str_Subclass;
	
	t.tp_hash = (hashfunc)[](OBJECT self) { return ism::hash((String)STR(self)); };
	
	t.tp_len = (lenfunc)[](OBJECT self) { return (ssize_t)STR(self).size(); };
	
	t.tp_repr = (reprfunc)[](OBJECT self) { return STR(self); };
	
	t.tp_str = (reprfunc)[](OBJECT self) { return STR(self); };

	t.tp_compare = (cmpfunc)[](OBJECT self, OBJECT other)
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

	t.tp_new = (newfunc)[](TYPE type, OBJECT args) -> OBJECT
	{
		return holder_type::new_();
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void StringObject::_bind_class(OBJECT scope)
{
	CLASS_<STR>(scope, "string", get_type_static())
		.def(init<>())
		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */