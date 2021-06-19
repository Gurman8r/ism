#include <core/api/object/property_object.hpp>
#include <core/api/class.hpp>

using namespace ism;
using namespace ism::api;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_STATIC_CLASS_TYPE(PropertyObject, t)
{
	t.tp_name = "property";
	t.tp_size = sizeof(PropertyObject);
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType;
	t.tp_base = typeof<OBJECT>();

	t.tp_alloc = (allocfunc)[](size_t size) { return memalloc(size); };
	t.tp_free = (freefunc)[](void * ptr) { memdelete((PropertyObject *)ptr); };

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v) { return util::compare(*o, *v); };

	t.tp_descr_get = (descrgetfunc)[](OBJECT descr, OBJECT obj, OBJECT type) -> OBJECT
	{
		return PROPERTY(descr)->m_get(obj);
	};

	t.tp_descr_set = (descrsetfunc)[](OBJECT descr, OBJECT obj, OBJECT value) -> Error
	{
		return PROPERTY(descr)->m_set(obj, value), Error_None;
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void PropertyObject::_bind_class(TypeObject & t)
{
	CLASS_<PROPERTY>(&t, "property")
		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */