#ifndef _ISM_PROJECT_SETTINGS_HPP_
#define _ISM_PROJECT_SETTINGS_HPP_

#include <core/api/super.hpp>

namespace ISM
{
	class ISM_API ProjectSettings : public Super
	{
		static ProjectSettings * singleton;

	public:
		FORCE_INLINE static ProjectSettings * get_singleton() { return singleton; }

		ProjectSettings() { singleton = this; }

		virtual ~ProjectSettings() { singleton = nullptr; }
	};

	NODISCARD inline ProjectSettings & get_project_settings() { return *CHECK(ProjectSettings::get_singleton()); }
}

#endif // !_ISM_PROJECT_SETTINGS_HPP_
