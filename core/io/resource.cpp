#include <core/io/resource.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(Resource, t, TypeFlags_IsAbstract)
	{
	}

	Resource::Resource() noexcept
	{
	}

	void Resource::set_path(String const & value, bool take_over)
	{
		if (take_over || m_path_cache != value) {
			m_path_cache = value;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}