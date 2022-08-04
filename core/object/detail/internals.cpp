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
		// set ALL internal object references to nullptr
		Vector<OBJ>{}.swap(loader_stack);
		modules = nullptr;
		while (!class_db.empty())
		{
			TYPE & t{ class_db.back<TYPE>() };
			if (!t) { continue; }
			t->tp_base = nullptr;
			t->tp_bases = nullptr;
			t->tp_dict = nullptr;
			t->tp_mro = nullptr;
			t->tp_subclasses = nullptr;
			class_db.pop_back();
		}
	}

	void Internals::add_class(TYPE const & type)
	{
		ASSERT(type);
		ASSERT(type.ready());
		size_t const type_id{ type->tp_name.hash_code() };
		ASSERT(!class_db.contains<size_t>(type_id));
		class_db.push_back(type_id, type->tp_name, type);
	}

	TYPE Internals::get_class(StringName const & name) const
	{
		size_t const i{ name.hash_code() };
		if (TYPE const * e{ class_db.map<size_t, TYPE>(i) }) {
			return *e;
		}
		return nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}