#include <core/io/resource_saver.hpp>

namespace ism
{
	ResourceSaver * ResourceSaver::__singleton{};

	ResourceSaver::ResourceSaver() { __singleton = this; }

	ResourceSaver::~ResourceSaver() {}

	Error_ ResourceSaver::save(RES const & res, String const & path)
	{
		return Error_OK;
	}
}