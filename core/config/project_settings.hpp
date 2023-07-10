#ifndef _ISM_PROJECT_SETTINGS_HPP_
#define _ISM_PROJECT_SETTINGS_HPP_

#include <core/object/var.hpp>
#include <core/io/config_file.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API ProjectSettings : public Object
	{
		OBJECT_CLASS(ProjectSettings, Object);

		SINGLETON_CLASS(ProjectSettings);

		HashMap<String, Var> m_data{};

		mutable String
			m_bin_path{},
			m_config_path{},
			m_mods_path{},
			m_project_path{},
			m_resource_path{};

	public:
		ProjectSettings() noexcept { SINGLETON_CTOR(); }
		virtual ~ProjectSettings() noexcept override { SINGLETON_DTOR(); }

		Error_ setup(String const & exec_path, String const & main_pack = {});

		NODISCARD Optional<Var> get(String const & path) const;
		Error_ set(String const & path, Optional<Var> const & value);

		NODISCARD String localize_path(String const & path) const;
		NODISCARD String globalize_path(String const & path) const;

		NODISCARD String get_bin_path() const noexcept;
		NODISCARD String get_config_path() const noexcept;
		NODISCARD String get_mods_path() const noexcept;
		NODISCARD String get_project_path() const noexcept;
		NODISCARD String get_resource_path() const noexcept;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	SINGLETON_WRAPPER(ProjectSettings, globals);

	// get global
#define GLOBAL_GET(path) \
		(Ism::globals()->get((path)))

	// set global
#define GLOBAL_SET(path, value) \
		(Ism::globals()->set((path), (value)))

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_PROJECT_SETTINGS_HPP_
