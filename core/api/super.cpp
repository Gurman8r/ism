#include <core/api/super.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Super::_postinitialize()
	{
		m_super_id = SuperDB::add_instance(this);
	}

	bool Super::_predelete()
	{
		m_predelete_ok = 1;
		if (m_predelete_ok) { /**/ }
		return m_predelete_ok;
	}

	Super::Super(bool is_ref) : m_is_reference{ is_ref }
	{
	}

	Super::~Super()
	{
		SuperDB::remove_instance(m_super_id);
		m_super_id = SuperID{};
		m_predelete_ok = 2;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	decltype(SuperDB::g_supers) SuperDB::g_supers{};

	SuperID SuperDB::add_instance(Super * value)
	{
		if (!value || g_supers.contains<Super *>(value)) { return SuperID{}; }

		static SuperID id{}; id++;
		g_supers.push_back(id, value);
		return id;
	}

	void SuperDB::remove_instance(SuperID id)
	{
		if (size_t i{ g_supers.index_of<SuperID>(id) }
		; i != g_supers.npos) { g_supers.erase(i); }
	}

	Super * SuperDB::get_instance(SuperID id)
	{
		return *g_supers.map<SuperID, Super *>(id);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}