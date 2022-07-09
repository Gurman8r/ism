#include <core/object/detail/internals.hpp>
#include <core/object/detail/class.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Internals * Internals::__singleton{};

	Internals::Internals()
	{
		__singleton = this;

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
		hash_t const type_id{ type->tp_name.hash_code() };
		ASSERT(!class_db.contains<hash_t>(type_id));
		class_db.push_back(type_id, type->tp_name, type);
	}

	TYPE Internals::get_class(StringName const & name) const
	{
		hash_t const i{ name.hash_code() };
		if (TYPE const * e{ class_db.map<hash_t, TYPE>(i) }) {
			return *e;
		}
		return nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}