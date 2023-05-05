#include <core/io/directory.hpp>
#include <core/os/os.hpp>
#include <core/config/project_settings.hpp>
#include <filesystem>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(Directory, t, TypeFlags_IsAbstract) {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Directory::CreateFunc Directory::__create_func{};

	Ref<Directory> Directory::create()
	{
		return nullptr;
	}

	Ref<Directory> Directory::create_for_path(String const & path)
	{
		return nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Ref<Directory> Directory::open(String const & path, Error_ * error)
	{
		return nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}