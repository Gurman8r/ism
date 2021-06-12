#include <core/api/super.hpp>
#include <core/api/modsupport.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	DECLEXPR(Super::g_superID) {};

	bool Super::_predelete()
	{
		m_predelete_ok = 1;
		if (m_predelete_ok) { m_class_ptr = nullptr; }
		return m_predelete_ok;
	}

	void Super::_postinitialize()
	{
		m_class_ptr = (StringName *)_get_class_namev();
	}

	void Super::_construct_super(bool is_ref)
	{
		m_is_reference = is_ref;
		m_instance_id = CHECK(++g_superID);
	}

	void Super::_destruct_super()
	{
		m_instance_id = {};
		m_predelete_ok = 2;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}