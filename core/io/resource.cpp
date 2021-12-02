#include <core/io/resource.hpp>

using namespace ism;

EMBEDED_CLASS(Resource, t, TypeFlags_IsAbstract)
{
}

Resource::Resource() noexcept { static RID next_rid{}; m_rid = ++next_rid; }

Resource::~Resource()
{
}

void Resource::reload_from_file()
{
}

void Resource::set_path(String const & value, bool take_over)
{
	if (m_path_cache == value) { return; }

	m_path_cache = value;
}