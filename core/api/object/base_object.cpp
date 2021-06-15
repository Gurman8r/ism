#include <core/api/object/base_object.hpp>
#include <core/api/modsupport.hpp>

using namespace ism;
using namespace ism::api;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void BaseObject::initialize_class()
{
	if (static bool once{}; !once && (once = true))
	{
		ClassDB::add_class<BaseObject>();

		_bind_class(BaseObject::ob_type_static);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

STATIC_MEMBER(BaseObject::ob_type_static) = COMPOSE(TypeObject, t)
{
	t.tp_name = "object";
	t.tp_size = sizeof(BaseObject);
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType;
	t.tp_base = nullptr;

	t.tp_getattro = (getattrofunc)generic_getattr;
	t.tp_setattro = (setattrofunc)generic_setattr;

	t.tp_alloc = (allocfunc)[](size_t size) { return memalloc(size); };
	t.tp_free = (freefunc)[](void * ptr) { memdelete((BaseObject *)ptr); };

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v) { return util::compare(*o, *v); };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void BaseObject::_bind_class(TypeObject & t)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

STATIC_MEMBER(ClassDB::classes) {};

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