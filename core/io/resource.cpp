#include <core/io/resource.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_OBJECT_CLASS(Resource, t, TypeFlags_IsAbstract)
	{
	}

	Error_ Resource::reload_from_file()
	{
		return Error_None;
	}

	void Resource::set_path(Path const & value, bool take_over)
	{
		if (m_path_cache == value) { return; }

		m_path_cache = value;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}