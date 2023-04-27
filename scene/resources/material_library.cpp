#include <scene/resources/material_library.hpp>

namespace ism
{
	Error_ MaterialLibrary::load_material(Material & material, Path const & path)
	{
		if (path.empty()) { return Error_Unknown; }

		return Error_OK;
	}

	Error_ MaterialLibrary::load_material(Ref<Material> material, Path const & path)
	{
		return !material ? Error_Unknown : load_material(**material, path);
	}
}