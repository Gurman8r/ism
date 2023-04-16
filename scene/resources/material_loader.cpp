#include <scene/resources/material_loader.hpp>

using namespace ism;

Error_ MaterialLoader::load_from_file(Material & material, Path const & path)
{
	if (!path) { return Error_Unknown; }

	if (isinstance<ShaderMaterial>(&material))
	{
	}
	else if (isinstance<StandardMaterial3D>(&material))
	{
	}
	else
	{
	}

	return Error_Unknown;
}

Error_ MaterialLoader::load_from_file(Ref<Material> material, Path const & path)
{
	if (!material) { return Error_Unknown; }
	return load_from_file(**material, path);
}
