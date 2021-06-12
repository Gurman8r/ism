#include <core/api/modsupport.hpp>

using namespace ism;
using namespace ism::api;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void BaseObject::initialize_class()
{
	static SCOPE_ENTER(&)
	{
		ClassDB::add_class<BaseObject>();

		_bind_methods(ob_type_static);
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

DECLEXPR(BaseObject::ob_type_static) = COMPOSE(TypeObject, t)
{
	t.tp_name = "object";
	t.tp_basicsize = sizeof(BaseObject);
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType;
	t.tp_base = nullptr;

	t.tp_getattr = (getattrfunc)api::impl_getattr_string;
	t.tp_setattr = (setattrfunc)api::impl_setattr_string;
	t.tp_getattro = (getattrofunc)api::impl_getattr_object;
	t.tp_setattro = (setattrofunc)api::impl_setattr_object;

	t.tp_alloc = (allocfunc)[](size_t size) { return memalloc(size); };
	t.tp_free = (freefunc)[](void * ptr) { memdelete((BaseObject *)ptr); };

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v) { return util::compare(*o, *v); };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void BaseObject::_bind_methods(TypeObject & t)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

DECLEXPR(ClassDB::classes) {};

void ClassDB::add_class(StringName const & name, TypeObject * type)
{
	VERIFY(name);
	VERIFY(type);
	VERIFY(type->ready());

	hash_t const id{ name.hash_code() };

	VERIFY(!classes.contains<hash_t>(id));

	classes.push_back(id, name, TYPE(type));
}

bool ClassDB::class_exists(StringName const & class_name)
{
	return classes.contains<hash_t>(hash(class_name));
}

TYPE ClassDB::class_type(StringName const & class_name)
{
	return classes.map_unchecked<hash_t, TYPE>(hash(class_name));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */