#include <core/io/resource.hpp>

using namespace ism;

EMBED_OBJECT_CLASS(Resource, t, TypeFlags_IsAbstract)
{
}

void Resource::reload_from_file()
{
}

void Resource::set_path(Path const & value, bool take_over)
{
	if (m_path_cache == value) { return; }

	m_path_cache = value;
}