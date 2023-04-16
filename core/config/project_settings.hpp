#ifndef _ISM_PROJECT_SETTINGS_HPP_
#define _ISM_PROJECT_SETTINGS_HPP_

#include <core/object/class.hpp>

namespace ism
{
	class ISM_API ProjectSettings : public Object
	{
		DEFINE_CLASS(ProjectSettings, Object);

		static ProjectSettings * __singleton;

	public:
		ProjectSettings() noexcept { __singleton = this; }
		virtual ~ProjectSettings() noexcept override = default;
		FORCE_INLINE static ProjectSettings * get_singleton() noexcept { return __singleton; }

	public:
		NODISCARD Path get_project_data_path() const;
		NODISCARD Path get_resource_path() const;
	};
}

#endif // !_ISM_PROJECT_SETTINGS_HPP_
