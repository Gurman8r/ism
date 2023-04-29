#ifndef _ISM_PROJECT_SETTINGS_HPP_
#define _ISM_PROJECT_SETTINGS_HPP_

#include <core/object/class.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API ProjectSettings : public Object
	{
		DEFINE_CLASS(ProjectSettings, Object);

		static ProjectSettings * __singleton;

		Path m_bin_path{}, m_cfg_path{}, m_dat_path{}, m_lib_path{}, m_res_path{}, m_usr_path{};

		HashMap<String, HashMap<String, OBJ>> m_data{};

	public:
		ProjectSettings() noexcept { __singleton = this; }
		virtual ~ProjectSettings() noexcept override = default;
		FORCE_INLINE static ProjectSettings * get_singleton() noexcept { return __singleton; }
#define PROJECT_SETTINGS (ism::ProjectSettings::get_singleton())

	public:
		Error_ setup(Path const & exepath, Path const & main_pack = {});

		NODISCARD Path globalize_path(Path const & path) const;
		NODISCARD Path get_binary_path(Path const & path = {}) const;
		NODISCARD Path get_config_path(Path const & path = {}) const;
		NODISCARD Path get_data_path(Path const & path = {}) const;
		NODISCARD Path get_library_path(Path const & path = {}) const;
		NODISCARD Path get_resource_path(Path const & path = {}) const;
		NODISCARD Path get_user_path(Path const & path = {}) const;

		NODISCARD OBJ get(String const & section, String const & name) const;
		Error_ set(String const & section, String const & name, OBJ const & value);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_PROJECT_SETTINGS_HPP_
