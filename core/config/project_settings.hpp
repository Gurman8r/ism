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

	public:
		ProjectSettings() noexcept { SINGLETON_CTOR(__singleton, this); }
		virtual ~ProjectSettings() noexcept override { SINGLETON_DTOR(__singleton, this); }
		FORCE_INLINE static ProjectSettings * get_singleton() noexcept { return __singleton; }

	public:
		Error_ setup(String const & exe_path, String const & main_pack = {});
		NODISCARD ObjectRef get(String const & section, String const & name) const;
		Error_ set(String const & section, String const & name, ObjectRef const & value);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	SINGLETON_WRAPPER(ProjectSettings, get_globals);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_PROJECT_SETTINGS_HPP_
