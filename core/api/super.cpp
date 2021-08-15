#include <core/api/super.hpp>

namespace ism
{
	static InstanceID g_super_id{};

	Super::Super(bool is_ref) noexcept
		: m_instance_id	{ CHECK(++g_super_id) }
		, m_class_ptr	{ CHECK(_get_class_namev()) }
		, m_is_reference{ is_ref }
	{
	}
}