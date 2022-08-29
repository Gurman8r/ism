#include <core/io/resource_loader.hpp>

namespace ism
{
	ResourceLoader * ResourceLoader::__singleton{};

	ResourceLoader::ResourceLoader()
	{
		__singleton = this;
	}

	ResourceLoader::~ResourceLoader()
	{
	}
}