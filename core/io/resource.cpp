#include <core/io/resource.hpp>
#include <core/io/resource_loader.hpp>
#include <core/io/resource_saver.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(Resource, t, TypeFlags_IsAbstract)
	{
	}

	void Resource::set_path(Path const & value, bool take_over)
	{
		if (take_over || m_path_cache != value) {
			m_path_cache = value;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RES load_resource(Path const & path)
	{
		return RESOURCE_LOADER->load(path);
	}

	Error_ save_resource(RES const & res, Path const & path)
	{
		return RESOURCE_SAVER->save(res, path);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}