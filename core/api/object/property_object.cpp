#include <core/api/object/property_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_OBJECT_TYPE_STATIC(PropertyObject, t)
{
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType | TypeFlags_MethodDescriptor;
	
	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v) { return util::compare(*o, *v); };

	t.tp_descr_get = (descrgetfunc)[](OBJECT self, OBJECT obj, OBJECT type) -> OBJECT
	{
		return PROPERTY(self).get(obj);
	};

	t.tp_descr_set = (descrsetfunc)[](OBJECT self, OBJECT obj, OBJECT value) -> Error
	{
		return PROPERTY(self).set(obj, value), Error_None;
	};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void PropertyObject::_bind_class(OBJECT scope)
{
	CLASS_<PROPERTY>(scope, "property", get_type_static())
		.def(init<>())
		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */