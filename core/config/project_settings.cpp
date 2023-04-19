#include <core/config/project_settings.hpp>

namespace ism
{
	EMBED_CLASS(ProjectSettings, t) {}

	ProjectSettings * ProjectSettings::__singleton{};

	Path ProjectSettings::get_project_data_path() const { return "../../data/"_path; }

	Path ProjectSettings::get_resource_path() const { return "../../res/"_path; }

	Path ProjectSettings::get_user_data_dir() const { return "../../user/"_path; }
}