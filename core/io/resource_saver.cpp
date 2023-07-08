#include <core/io/resource_saver.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(ResourceFormatSaver, t) {}

	bool ResourceFormatSaver::recognize_path(String const & path, String const & hint) const
	{
		Vector<String> recognized_extensions{};
		get_recognized_extensions(&recognized_extensions);
		return recognized_extensions.contains(path.extension());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ResourceSaver * ResourceSaver::__singleton{};

	Error_ ResourceSaver::save(RES const & value, String const & path, i32 flags)
	{
		auto & savers{ get_singleton()->m_savers };
		Error_ error{};
		for (size_t i{}; i < savers.size(); ++i)
		{
			if (!savers[i]->recognize_path(path)) {
				continue;
			}

			if ((error = savers[i]->save(value, path, flags)) == Error_OK) {
				return Error_OK;
			}
		}
		return error;
	}

	bool ResourceSaver::add_resource_format_saver(Ref<ResourceFormatSaver> format)
	{
		auto & savers{ get_singleton()->m_savers };
		if (savers.contains(format)) {
			return false;
		}
		savers.push_back(format);
		return true;
	}

	bool ResourceSaver::remove_resource_format_saver(Ref<ResourceFormatSaver> format)
	{
		auto & savers{ get_singleton()->m_savers };
		if (auto const it{ savers.find(format) }; it != savers.end()) {
			savers.erase(it);
			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}