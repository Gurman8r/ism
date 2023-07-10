#include <scene/resources/material_library.hpp>
#include <core/config/project_settings.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(MaterialFormatLoader, t) {}

	Error_ MaterialFormatLoader::load_material(Ref<Material> material, String const & path)
	{
		if (!material) { return Error_Failed; }
		if (path.empty()) { return Error_Failed; }
		return Error_OK;
	}

	RES MaterialFormatLoader::load(String const & path, Error_ * r_error)
	{
		Ref<Material> temp{}; //temp.instance();
		if (Error_ const err{ load_material(temp, path) }) {
			if (r_error) { *r_error = err; }
			temp = nullptr;
		}
		else if (r_error) { *r_error = Error_OK; }
		return temp;
	}

	void MaterialFormatLoader::get_recognized_extensions(Vector<String> * out) const
	{
		if (!out) { return; }
		out->push_back(".mat");
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}