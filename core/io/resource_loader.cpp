#include <core/io/resource_loader.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(ResourceFormatLoader, t) {}

	bool ResourceFormatLoader::recognize_path(String const & path, String const & hint) const
	{
		Vector<String> recognized_extensions{};
		get_recognized_extensions(&recognized_extensions);
		return recognized_extensions.contains(path.extension());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ResourceLoader * ResourceLoader::__singleton{};
	
	RES ResourceLoader::_load(String const & path, Error_ * r_error)
	{
		auto & loaders{ get_singleton()->m_loaders };

		bool found{};
		RES result{};
		for (size_t i{}; i < loaders.size(); ++i) {
			if (!loaders[i]->recognize_path(path)) { continue; }
			found = true;
			if (result = loaders[i]->load(path, r_error)) { break; }
		}
		if (result) {
			return result;
		}

		return nullptr;
	}
	
	RES ResourceLoader::load(String const & path, Error_ * r_error)
	{
		return _load(path, r_error);
	}

	bool ResourceLoader::add_resource_format_loader(Ref<ResourceFormatLoader> format)
	{
		auto & loaders{ get_singleton()->m_loaders };
		if (loaders.contains(format)) {
			return false;
		}
		loaders.push_back(format);
		return true;
	}

	bool ResourceLoader::remove_resource_format_loader(Ref<ResourceFormatLoader> format)
	{
		auto & loaders{ get_singleton()->m_loaders };
		if (auto const it{ loaders.find(format) }; it != loaders.end()) {
			loaders.erase(it);
			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}