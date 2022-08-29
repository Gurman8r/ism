#include <core/io/resource_saver.hpp>

namespace ism
{
	ResourceSaver * ResourceSaver::__singleton{};

	ResourceSaver::ResourceSaver()
	{
		__singleton = this;
	}

	ResourceSaver::~ResourceSaver()
	{
	}
}