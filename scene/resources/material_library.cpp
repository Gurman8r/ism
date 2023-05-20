#include <scene/resources/material_library.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(MaterialFormatLoader, t) {}

	Error_ MaterialFormatLoader::load_material(Ref<Material> material, String const & path)
	{
		if (!material) { return Error_Failed; }
		if (path.empty()) { return Error_Failed; }
		return Error_OK;
	}

	RES MaterialFormatLoader::load(String const & path, Error_ * r_error)
	{
		Ref<Material> temp{}; //temp.instance();
		if (auto const error{ load_material(temp, path) }) { if (r_error) { *r_error = error; } temp = nullptr; }
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