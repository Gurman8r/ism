#include <core/io/dir_access.hpp>
#include <core/os/os.hpp>
#include <core/config/project_settings.hpp>
#include <filesystem>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(DirAccess, t, TypeFlags_IsAbstract) {};

	DirAccess::DirAccess() {}

	DirAccess::~DirAccess() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}