#include <core/api/internals.hpp>
#include <core/api/class.hpp>

using namespace ism;

DECLEXPR(Internals::singleton) {};

void Internals::initialize()
{
	dict = DICT::new_();
	builtins = DICT::new_();
	modules = DICT::new_();
	importlib = DICT::new_();
	import_func = nullptr;
}

void Internals::finalize()
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

	VERIFY(!classes.contains<hash_t>(id));

	classes.push_back(id, name, type);
}