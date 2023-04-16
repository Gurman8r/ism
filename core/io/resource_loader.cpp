#include <core/io/resource_loader.hpp>

namespace ism
{
	EMBED_CLASS(ResourceFormatLoader, t) {}

	ResourceFormatLoader::~ResourceFormatLoader() {}
}

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