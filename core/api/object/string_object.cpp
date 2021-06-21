#include <core/api/object/string_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_OBJECT_TYPE_STATIC(StringObject, t)
{
	t.tp_flags = TypeFlags_Default | TypeFlags_Str_Subclass;
	
	t.tp_hash = (hashfunc)[](OBJECT o) { return ism::hash((String)STR(o)); };
	
	t.tp_len = (lenfunc)[](OBJECT o) { return (ssize_t)STR(o).size(); };
	
	t.tp_repr = (reprfunc)[](OBJECT o) { return STR(o); };
	
	t.tp_str = (reprfunc)[](OBJECT o) { return STR(o); };

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v)
	{
		if (STR::check_(v))
		{
			return util::compare((String)STR(o), (String)STR(v));
		}
		else if (STR s{ { v } })
		{
			return util::compare((String)STR(o), (String)s);
		}
		else
		{
			return util::compare(*o, *v);
		}
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