#include <core/api/object/string_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_OBJECT_TYPE_STATIC(StringObject, t)
{
	t.tp_name = "string";
	t.tp_size = sizeof(StringObject);
	t.tp_flags = TypeFlags_Default | TypeFlags_Str_Subclass;
	t.tp_base = typeof<OBJECT>();

	t.tp_hash = (hashfunc)[](OBJECT o) { return ism::hash((String)STR(o)); };
	t.tp_len = (lenfunc)[](OBJECT o) { return (ssize_t)STR(o).size(); };
	t.tp_repr = (reprfunc)[](OBJECT o) { return STR(o); };
	t.tp_str = (reprfunc)[](OBJECT o) { return STR(o); };

	t.tp_alloc = (allocfunc)[](size_t size) { return memalloc(size); };
	t.tp_free = (freefunc)[](void * ptr) { memdelete((StringObject *)ptr); };

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v)
	{
		if (STR::check_(v))
		{
			return util::compare((String)STR(o), (String)STR(v));
		}
		else if (auto s{ STR({ v }) })
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

void StringObject::_bind_class(TypeObject & t)
{
	CLASS_<STR>(&t, "string")
		//.def(init<>())
		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */