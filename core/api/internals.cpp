#include <core/api/internals.hpp>
#include <core/api/class.hpp>

using namespace ism::api;

MEMBER_IMPL(Internals::singleton) {};

Internals::Internals()
{
	singleton = this;
	dict = DICT::new_();
	builtins = DICT::new_();
	modules = DICT::new_();
}

Internals::~Internals()
{
	dict = nullptr;
	builtins = nullptr;
	modules = nullptr;
}

void Internals::add_class(TYPE type)
{
	VERIFY(type);
	VERIFY(type.ready());

	String const & name{ type->tp_name };
	hash_t const id{ ism::hash(name) };

	VERIFY(!class_db.contains<hash_t>(id));
	class_db.push_back(id, name, type);
}