#include <core/object/detail/internals.hpp>
#include <core/object/detail/class.hpp>

using namespace ism;

VAR_IMPL(Internals::singleton) {};

Internals::Internals()
{
	singleton = this;
	dict = DICT::new_();
	builtins = DICT::new_();
	modules = DICT::new_();
	importlib = DICT::new_();
	import_func = nullptr;
}

Internals::~Internals()
{
	dict = nullptr;
	builtins = nullptr;
	modules = nullptr;
	importlib = nullptr;
	import_func = nullptr;
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