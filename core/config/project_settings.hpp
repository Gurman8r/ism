#ifndef _ISM_PROJECT_SETTINGS_HPP_
#define _ISM_PROJECT_SETTINGS_HPP_

#include <core/io/config_file.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API ProjectSettings : public Object
	{
		DEFINE_CLASS(ProjectSettings, Object);

		static ProjectSettings * __singleton;

		HashMap<String, HashMap<String, ObjectRef>> m_data{};

		mutable String
			m_bin_path{},
			m_config_path{},
			m_mods_path{},
			m_project_path{},
			m_resource_path{};

	public:
		ProjectSettings() noexcept { SINGLETON_CTOR(__singleton, this); }
		virtual ~ProjectSettings() noexcept override { SINGLETON_DTOR(__singleton, this); }
		SINGLETON_GETTER(ProjectSettings, __singleton);

	public:
		Error_ setup(String const & exec_path, String const & main_pack = {});

	public:
		NODISCARD ObjectRef get(String const & section, String const & name) const;
		Error_ set(String const & section, String const & name, ObjectRef const & value);

	public:
		NODISCARD String localize_path(String const & path) const;
		NODISCARD String globalize_path(String const & path) const noexcept;

		NODISCARD String get_project_path() const noexcept;
		NODISCARD String get_bin_path() const noexcept;
		NODISCARD String get_config_path() const noexcept;
		NODISCARD String get_mods_path() const noexcept;
		NODISCARD String get_resource_path() const noexcept;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	SINGLETON_WRAPPER(ProjectSettings, get_globals);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_PROJECT_SETTINGS_HPP_
