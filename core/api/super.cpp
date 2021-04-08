#include <core/api/super.hpp>
#include <core/api/internals.hpp>

namespace ISM
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Super::_postinitialize() {}

	bool Super::_predelete()
	{
		m_predelete_ok = 1;
		if (m_predelete_ok) { /**/ }
		return m_predelete_ok;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Super::Super(bool is_ref)
	{
		m_is_reference = is_ref;
		m_super_id = register_super_instance(this);
		VERIFY(m_super_id);
	}

	Super::~Super()
	{
		unregister_super_instance(this);
		m_super_id = nullptr;
		m_predelete_ok = 2;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Super * Super::get_super_instance(SuperID id)
	{
		if (!id) { return nullptr; }
		
		auto & super_data{ get_internals().super_data };

		if (size_t i{ super_data.index_of<SuperID>(id) }; i != super_data.npos)
		{
			return super_data.get<Super *>(i);
		}

		return nullptr;
	}

	SuperID Super::register_super_instance(Super * value)
	{
		if (!value) { return nullptr; }

		auto & super_data{ get_internals().super_data };
		
		static SuperID id{};

		super_data.push_back(++id, value);

		return id;
	}

	void Super::unregister_super_instance(Super * value)
	{
		if (!value) { return; }

		auto & super_data{ get_internals().super_data };

		SuperID id{ value->get_super_id() };

		if (!id) { return; }

		if (size_t i{ super_data.index_of<SuperID>(id) }; i != super_data.npos)
		{
			super_data.erase(i);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}