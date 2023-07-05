#ifndef _ISM_PROJECT_SETTINGS_HPP_
#define _ISM_PROJECT_SETTINGS_HPP_

#include <core/object/class.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API ProjectSettings : public Object
	{
		DEFINE_CLASS(ProjectSettings, Object);

		static ProjectSettings * __singleton;

		String
			m_bin_path			{ "./bin/" },
			m_cache_path		{ "./cache/" },
			m_config_path		{ "./config/" },
			m_data_path			{ "./data/" },
			m_defaults_path		{ "./defaultconfigs/" },
			m_downloads_path	{ "./downloads/" },
			m_mods_path			{ "./mods/" },
			m_profiles_path		{ "./profiles/" },
			m_resources_path	{ "./resources/" },
			m_saves_path		{ "./saves/" },
			m_user_path			{ "./user/" };

		HashMap<String, HashMap<String, ObjectRef>> m_data{};

	public:
		ProjectSettings() noexcept { SINGLETON_CTOR(); }
		virtual ~ProjectSettings() noexcept override { SINGLETON_DTOR(); }
		FORCE_INLINE static ProjectSettings * get_singleton() noexcept { return __singleton; }

	public:
		Error_ setup(String const & exepath, String const & main_pack = {});

	public:
		NODISCARD String globalize_path(String const & path) const;
		NODISCARD String get_bin_path() const;
		NODISCARD String get_cache_path() const;
		NODISCARD String get_config_path() const;
		NODISCARD String get_data_path() const;
		NODISCARD String get_downloads_path() const;
		NODISCARD String get_mods_path() const;
		NODISCARD String get_profiles_path() const;
		NODISCARD String get_resources_path() const;
		NODISCARD String get_saves_path() const;
		NODISCARD String get_user_path() const;

	public:
		NODISCARD ObjectRef get(String const & section, String const & name) const;
		Error_ set(String const & section, String const & name, ObjectRef const & value);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	SINGLETON_WRAPPER(ProjectSettings, get_project_settings);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_PROJECT_SETTINGS_HPP_
