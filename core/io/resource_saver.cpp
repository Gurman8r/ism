#include <core/io/resource_saver.hpp>
#include <core/config/project_settings.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(ResourceFormatSaver, t) {}

	bool ResourceFormatSaver::recognize_path(String const & path, String const & hint) const
	{
		Vector<String> recognized_extensions{};
		get_recognized_extensions(&recognized_extensions);
		return recognized_extensions.contains(path.extension());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(ResourceSaver, t) {}

	SINGLETON_EMBED(ResourceSaver);

	Error_ ResourceSaver::save(RES const & value, String const & path, i32 flags)
	{
		String const gpath{ globals()->globalize_path(path).replace('\\', '/') };

		Error_ error{};
		for (size_t i{}; i < m_savers.size(); ++i)
		{
			if (!m_savers[i]->recognize_path(gpath)) {
				continue;
			}

			if ((error = m_savers[i]->save(value, gpath, flags)) == Error_OK) {
				return Error_OK;
			}
		}
		return error;
	}

	bool ResourceSaver::add_resource_format_saver(Ref<ResourceFormatSaver> format)
	{
		if (m_savers.contains(format)) {
			return false;
		}
		m_savers.push_back(format);
		return true;
	}

	bool ResourceSaver::remove_resource_format_saver(Ref<ResourceFormatSaver> format)
	{
		if (auto const it{ m_savers.find(format) }; it != m_savers.end()) {
			m_savers.erase(it);
			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}