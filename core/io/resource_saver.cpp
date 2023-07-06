#include <core/io/resource_saver.hpp>

namespace Ism
{
	ResourceSaver * ResourceSaver::__singleton{};

	ResourceSaver::ResourceSaver() { SINGLETON_CTOR(__singleton, this); }

	ResourceSaver::~ResourceSaver() { SINGLETON_DTOR(__singleton, this); }

	Error_ ResourceSaver::save(RES const & res, String const & path)
	{
		return Error_OK;
	}
}