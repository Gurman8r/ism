#include <core/api/super.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Super::_predelete()
	{
		m_predelete_ok = 1;

		if (m_predelete_ok) { m_class_ptr = nullptr; }

		return m_predelete_ok;
	}

	void Super::_postinitialize()
	{
		m_class_ptr = _get_class_namev();

		m_instance_id = SuperDB::add_instance(this);
	}

	Super::Super(bool is_ref) { m_is_reference = is_ref; }
	
	Super::Super() : Super{ false } {}

	Super::~Super()
	{
		SuperDB::remove_instance(m_instance_id);

		m_instance_id = InstanceID{};

		m_predelete_ok = 2;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	decltype(SuperDB::g_supers) SuperDB::g_supers{};

	InstanceID SuperDB::add_instance(Super * value)
	{
		if (!value || g_supers.contains<Super *>(value)) { return InstanceID{}; }

		static InstanceID id{}; ++id;
		
		g_supers.push_back(id, value);
		
		return id;
	}

	void SuperDB::remove_instance(InstanceID id)
	{
		if (size_t i{ g_supers.index_of<InstanceID>(id) }; i != g_supers.npos)
		{
			g_supers.erase(i);
		}
	}

	Super * SuperDB::get_instance(InstanceID id)
	{
		return *g_supers.map<InstanceID, Super *>(id);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}