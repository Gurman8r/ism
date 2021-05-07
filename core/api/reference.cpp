#include <core/api/reference.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Reference::~Reference() {}

	Reference::Reference() : Super{ true } { m_refcount.init(); m_refcount_init.init(); }

	bool Reference::init_ref()
	{
		if (inc_ref())
		{
			if (!has_references() && m_refcount_init.unref())
			{
				dec_ref(); // first referencing is already 1, so compensate for the ref above
			}
			return true;
		}
		return false;
	}

	bool Reference::inc_ref() // returns false if refcount is at zero and didn't get increased
	{
		uint32_t rc_val{ m_refcount.refval() };
		bool success{ rc_val != 0 };
		if (success && rc_val <= 2 /* higher is not relevant */)
		{
			// TODO...
		}
		return success;
	}

	bool Reference::dec_ref()
	{
		uint32_t rc_val{ m_refcount.unrefval() };
		bool die{ rc_val == 0 };
		if (rc_val <= 1 /* higher is not relevant */)
		{
			// TODO...
		}
		return die;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Any WeakRef::get_ref() const
	{
		if (!m_reference) { return nullptr; }

		Super * o{ SuperDB::get_instance(m_reference) };
		if (!o) { return nullptr; }

		Reference * r{ super_cast<Reference>(o) };
		if (r) { return REF{ r }; }

		return o;
	}

	void WeakRef::set_obj(Super * value)
	{
		m_reference = value ? value->get_instance_id() : InstanceID{};
	}

	void WeakRef::set_ref(REF const & value)
	{
		m_reference = value ? value->get_instance_id() : InstanceID{};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}