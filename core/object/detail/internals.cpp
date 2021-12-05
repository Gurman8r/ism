#include <core/object/detail/internals.hpp>
#include <core/object/detail/class.hpp>

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
	ASSERT(type);
	ASSERT(type.ready());

	String const & name{ type->tp_name };
	hash_t const id{ hash(name) };

	ASSERT(!class_db.contains<hash_t>(id));
	class_db.push_back(id, name, type);
}

TYPE Internals::get_class(StringName const & name) const
{
	hash_t const i{ hash(name) };
	
	if (TYPE const * e{ class_db.map<hash_t, TYPE>(i) }) { return *e; }
	
	return nullptr;
}