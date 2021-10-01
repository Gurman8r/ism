#include <core/api/internals.hpp>
#include <core/api/class.hpp>

using namespace ism;

MEMBER_IMPL(Internals::singleton) {};

Internals::Internals()
{
	singleton = this;

	modules = DICT::new_();
}

Internals::~Internals()
{
	modules = nullptr;
}

void Internals::add_class(TYPE const & type)
{
	VERIFY(type);
	VERIFY(type.ready());

	String const & name{ type->tp_name };
	hash_t const id{ hash(name) };

	VERIFY(!class_db.contains<hash_t>(id));
	class_db.push_back(id, name, type);
}

TYPE Internals::get_class(StringName const & name) const
{
	hash_t const id{ hash(name) };

	if (TYPE const * item{ class_db.map<hash_t, TYPE>(id) })
	{
		return *item;
	}

	return nullptr;
}