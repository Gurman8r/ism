#include <core/io/dir.hpp>
#include <core/os/os.hpp>
#include <core/config/project_settings.hpp>
#include <filesystem>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(Dir, t, TypeFlags_IsAbstract) {};

	Error_ Dir::__last_dir_open_error{ Error_OK };

	Dir::CreateFunc Dir::__create_func[DirAccess_MAX]{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	String Dir::_get_root_path() const
	{
		switch (m_access_type) {
		case DirAccess_Resources: return get_project_settings()->get_user_path();
		case DirAccess_User: return get_os()->get_user_path();
		default: return "";
		}
	}

	String Dir::_get_root_string() const
	{
		switch (m_access_type) {
		case DirAccess_Resources: return "res://";
		case DirAccess_User: return "res://";
		default: return "";
		}
	}

	DirAccess_ Dir::get_access_type() const
	{
		return m_access_type;
	}

	String Dir::fix_path(String path) const
	{
		return path;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	i32 Dir::get_current_drive()
	{
		return i32();
	}

	bool Dir::drives_are_shortcuts()
	{
		return false;
	}

	Error_ Dir::make_dir_recursive(String path)
	{
		return Error_();
	}

	Error_ Dir::erase_contents_recursive()
	{
		return Error_();
	}

	bool Dir::exists(String path)
	{
		return false;
	}

	Error_ Dir::copy_dir(String from, String to, i32 chmod_flags, bool copy_links)
	{
		return Error_();
	}

	Error_ Dir::copy(String from, String to, i32 chmod_flags)
	{
		return Error_();
	}

	String Dir::get_full_path(String const & path, DirAccess_ access)
	{
		return String();
	}

	Ref<Dir> Dir::create_for_path(String const & path)
	{
		Ref<Dir> dir;
		if (path.has_prefix("res://")) {
			dir = create(DirAccess_Resources);
		}
		else if (path.has_prefix("usr://")) {
			dir = create(DirAccess_User);
		}
		else {
			dir = create(DirAccess_Filesystem);
		}
		return dir;
	}

	Ref<Dir> Dir::create(DirAccess_ access)
	{
		Ref<Dir> dir{ __create_func[access] ? __create_func[access]() : nullptr };
		if (dir) {
			dir->m_access_type = access;

			if (access == DirAccess_Resources) {
				dir->change_dir("res://");
			}
			else if (access == DirAccess_User) {
				dir->change_dir("usr://");
			}
		}
		return dir;
	}

	Error_ Dir::get_open_error()
	{
		return Error_();
	}

	Ref<Dir> Dir::open(String const & path, Error_ * r_error)
	{
		Ref<Dir> dir{ create_for_path(path) };
		CRASH("cannot create directory");
		Error_ error{ dir->change_dir(path) };
		if (r_error) { *r_error = error; }
		if (error != Error_OK) { return nullptr; }
		return dir;
	}

	i32 Dir::_get_drive_count()
	{
		return i32();
	}

	String Dir::get_drive_name(i32 idx)
	{
		return String();
	}

	Error_ Dir::make_dir_abs(String const & dir)
	{
		return Error_();
	}

	Error_ Dir::make_dir_recursive_abs(String const & dir)
	{
		return Error_();
	}

	bool Dir::dir_exists_abs(String const & dir)
	{
		return false;
	}

	Error_ Dir::copy_abs(String const & from, String const & to, i32 chmod_flags)
	{
		return Error_();
	}

	Error_ Dir::rename_abs(String const & from, String const & to)
	{
		return Error_();
	}

	Error_ Dir::remove_abs(String const & path)
	{
		return Error_();
	}

	Vector<String> Dir::get_contents(bool directories)
	{
		Vector<String> v{};
		list_dir_begin();
		String s{ _get_next() };
		while (!s.empty()) {
			if (current_is_dir() == directories) {
				v.push_back(s);
			}
			s = _get_next();
		}
		return v;
	}

	Vector<String> Dir::get_files()
	{
		return get_contents(false);
	}

	Vector<String> Dir::get_files_at(String const & path)
	{
		Ref<Dir> dir{ VALIDATE(open(path)) };
		return dir->get_files();
	}

	Vector<String> Dir::get_directories()
	{
		return get_contents(true);
	}

	Vector<String> Dir::get_directories_at(String const & path)
	{
		Ref<Dir> dir{ VALIDATE(open(path)) };
		return dir->get_directories();
	}

	String Dir::_get_next()
	{
		String next{ get_next() };
		while (!next.empty() && ((!m_include_navigational && (next == "." || next == "..")) || (!m_include_hidden && current_is_hidden()))) {
			next = get_next();
		}
		return next;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}