#include <core/object/detail/internals.hpp>
#include <core/object/detail/class.hpp>

using namespace ism;

MEMBER_IMPL(Internals::singleton) {};

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

	VERIFY(!class_db.contains<hash_t>(id));

	class_db.push_back(id, name, type);
}