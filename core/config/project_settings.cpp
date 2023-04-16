#include <core/config/project_settings.hpp>

namespace ism
{
	EMBED_CLASS(ProjectSettings, t) {}

	ProjectSettings * ProjectSettings::__singleton{};

	Path ProjectSettings::get_project_data_path() const { return ""_path; }

	Path ProjectSettings::get_resource_path() const { return ""_path; }
}