#include <core/io/resource.hpp>

using namespace ism;

OBJECT_IMPL(Resource, t, TypeFlags_BaseType | TypeFlags_IsAbstract)
{
}

static RID g_resource_id{};

Resource::Resource() noexcept : Object{} { m_resource_id = ++g_resource_id; }

Resource::~Resource() {}

void Resource::reload_from_file()
{
}

void Resource::set_path(String const & value, bool take_over)
{
	m_path_cache = value;
}