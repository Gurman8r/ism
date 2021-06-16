#include <core/api/class_db.hpp>
#include <core/api/object/generic_object.hpp>

using namespace ism;
using namespace ism::api;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

DECLEXPR(ClassDB::classes) {};

void ClassDB::add_class(StringName const & name, TypeObject * type)
{
	VERIFY(!name.empty());
	VERIFY(type);
	VERIFY(type->ready());

	hash_t const id{ ism::hash(name) };
	VERIFY(!classes.contains<hash_t>(id));
	classes.push_back(id, name, TYPE(type));
}

bool ClassDB::class_exists(StringName const & name)
{
	return classes.contains<hash_t>(ism::hash(name));
}

TYPE ClassDB::class_type(StringName const & name)
{
	return classes.map_unchecked<hash_t, TYPE>(ism::hash(name));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */