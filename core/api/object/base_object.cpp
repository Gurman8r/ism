#include <core/api/modsupport.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void CoreObject::initialize_class()
{
	static SCOPE_ENTER(&)
	{
		ClassDB::add_class<CoreObject>();

		_bind_methods(ob_type_static);
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

DECLEXPR(CoreObject::ob_type_static) = COMPOSE(CoreType, t)
{
	t.tp_name = "object";
	t.tp_basicsize = sizeof(CoreObject);
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType;
	t.tp_base = nullptr;

	t.tp_getattr = (getattrfunc)detail::impl_getattr_string;
	t.tp_setattr = (setattrfunc)detail::impl_setattr_string;
	t.tp_getattro = (getattrofunc)detail::impl_getattr_object;
	t.tp_setattro = (setattrofunc)detail::impl_setattr_object;

	t.tp_alloc = (allocfunc)[](size_t size) { return memalloc(size); };
	t.tp_free = (freefunc)[](void * ptr) { memdelete((CoreObject *)ptr); };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void CoreObject::_bind_methods(CoreType & t)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

DECLEXPR(ClassDB::classes) {};

void ClassDB::add_class(StringName const & name, CoreType * type)
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
	return classes.contains<hash_t>(class_name.hash_code());
}

TYPE ClassDB::class_type(StringName const & class_name)
{
	return classes.map_unchecked<hash_t, TYPE>(class_name.hash_code());
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */