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

		HashMap<String, HashMap<String, OBJ>> m_data{};

	public:
		ProjectSettings() noexcept { __singleton = this; }
		virtual ~ProjectSettings() noexcept override = default;
		FORCE_INLINE static ProjectSettings * get_singleton() noexcept { return __singleton; }
#define PROJECT_SETTINGS (ism::ProjectSettings::get_singleton())

	public:
		Error_ setup(Path const & exepath, Path const & main_pack = {});

		NODISCARD Path get_bin_path() const;
		NODISCARD Path get_data_path() const;
		NODISCARD Path get_etc_path() const;
		NODISCARD Path get_library_path() const;
		NODISCARD Path get_resource_path() const;
		NODISCARD Path get_user_path() const;

		NODISCARD OBJ get(String const & section, String const & name) const;
		Error_ set(String const & section, String const & name, OBJ const & value);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_PROJECT_SETTINGS_HPP_
