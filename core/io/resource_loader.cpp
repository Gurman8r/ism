#include <core/io/resource_loader.hpp>
#include <core/config/project_settings.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(ResourceFormatLoader, t) {}

	bool ResourceFormatLoader::recognize_path(String const & path, String const & hint) const
	{
		Vector<String> recognized_extensions{};
		get_recognized_extensions(&recognized_extensions);
		return recognized_extensions.contains(path.extension());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(ResourceLoader, t) {}

	SINGLETON_EMBED(ResourceLoader);
	
	RES ResourceLoader::load(String const & path, Error_ * r_error)
	{
		String const gpath{ globals()->globalize_path(path).replace('\\', '/') };

		bool found{};
		RES result{};
		for (size_t i{}; i < m_loaders.size(); ++i)
		{
			if (!m_loaders[i]->recognize_path(gpath)) {
				continue;
			}
			found = true;
			if (result = m_loaders[i]->load(gpath, r_error)) {
				break;
			}
		}
		if (result) {
			return result;
		}
		return nullptr;
	}

	bool ResourceLoader::add_resource_format_loader(Ref<ResourceFormatLoader> format)
	{
		if (m_loaders.contains(format)) {
			return false;
		}
		m_loaders.push_back(format);
		return true;
	}

	bool ResourceLoader::remove_resource_format_loader(Ref<ResourceFormatLoader> format)
	{
		if (auto const it{ m_loaders.find(format) }; it != m_loaders.end()) {
			m_loaders.erase(it);
			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}