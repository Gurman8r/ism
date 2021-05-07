#include <core/api/types/base_object.hpp>
#include <core/api/modsupport.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

DECLEXPR(ClassDB::classes) {};

void ClassDB::add_class(StringName const & name, TYPE t)
{
	VERIFY(name);

	hash_t const id{ name.hash_code() };

	VERIFY(!classes.contains<hash_t>(id));

	classes.push_back(id, name, t);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

DECLEXPR(CoreObject::ob_class) { nullptr };

DECLEXPR(CoreObject::g_none) { nullptr };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool CoreObject::_predelete()
{
	if (!Super::_predelete()) { return false; }

	return true;
}

void CoreObject::_postinitialize()
{
	Super::_postinitialize();

	_initialize_classv();
}

void CoreObject::initialize_class()
{
	static SCOPE_ENTER(&)
	{
		ClassDB::add_class<CoreObject>();

		_bind_class(ob_class);
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void CoreObject::_bind_class(CoreType & t)
{
	t.tp_name = "object";
	t.tp_basicsize = sizeof(CoreObject);
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType;

	t.tp_getattr = (getattrfunc)detail::impl_getattr_string;
	t.tp_setattr = (setattrfunc)detail::impl_setattr_string;
	t.tp_getattro = (getattrofunc)detail::impl_getattr_object;
	t.tp_setattro = (setattrofunc)detail::impl_setattr_object;

	t.tp_operator_new = (allocfunc)[](size_t size) { return memalloc(size); };
	t.tp_operator_delete = (freefunc)[](void * ptr) { memdelete((CoreObject *)ptr); };
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */