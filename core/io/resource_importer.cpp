#include <core/io/resource_importer.hpp>

namespace ism
{
	ResourceImporter * ResourceImporter::__singleton{};

	ResourceImporter::ResourceImporter()
	{
		__singleton = this;
	}

	ResourceImporter::~ResourceImporter()
	{
	}
}