#include <core/object/detail/internals.hpp>
#include <core/object/detail/class.hpp>

using namespace ism;

MEMBER_IMPL(Internals::g_singleton) {};

Internals::Internals()
{
	g_singleton = this;

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

	String const & type_name{ type->tp_name };
	hash_t const type_id{ hash(type_name.data(), type_name.size()) };

	ASSERT(!class_db.contains<hash_t>(type_id));
	class_db.push_back(type_id, type_name, type);
}

TYPE Internals::get_class(StringName const & name) const
{
	hash_t const i{ name.hash_code() };
	
	if (TYPE const * e{ class_db.map<hash_t, TYPE>(i) }) { return *e; }
	
	return nullptr;
}