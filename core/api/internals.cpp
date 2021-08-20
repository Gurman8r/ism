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

void Internals::add_class(StringName const & name, TYPE type)
{
	VERIFY(!name.empty());

	VERIFY(type);

	VERIFY(type.ready());

	hash_t const id{ ism::hash(name) };

	VERIFY(!singleton.classes.contains<hash_t>(id));

	singleton.classes.push_back(id, name, type);
}