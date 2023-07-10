#include <core/io/dir.hpp>
#include <core/os/os.hpp>
#include <core/config/project_settings.hpp>
#include <core/io/file.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(Dir, t, TypeFlags_IsAbstract) {};

	Error_ Dir::__last_dir_open_error{ Error_OK };

	Dir::CreateFunc Dir::__create_func[DirAccess_MAX]{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	String Dir::_get_root_path() const
	{
		switch (m_access_type) {
		case DirAccess_Resources: return globals()->get_resource_path();
		case DirAccess_User: return os()->get_user_path();
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
		switch (m_access_type)
		{
		case DirAccess_Resources: {
			if (path.begins_with("res://")) {
				String const resource_path{ os()->get_resource_path() };
				if (!resource_path.empty()) { return path.replace_first("res:/", resource_path); }
				return path.replace_first("res://", "");
			}
		} break;
		case DirAccess_User: {
			if (path.begins_with("user://")) {
				String const data_dir{ OS::get_singleton()->get_user_path() };
				if (!data_dir.empty()) { return path.replace_first("user:/", data_dir); }
				return path.replace_first("user://", "");
			}
		} break;
		case DirAccess_Filesystem: {
			return path;
		} break;
		case DirAccess_MAX: {
			/* can't happen, but silences warning */
		} break;
		}
		return path;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	i32 Dir::get_current_drive()
	{
		String const path{ get_current_dir().lowercase() };
		for (i32 i{}; i < get_drive_count(); ++i) {
			String const drive{ get_drive(i).lowercase() };
			if (path.begins_with(drive)) {
				return i;
			}
		}
		return 0;
	}

	bool Dir::drives_are_shortcuts()
	{
		return false;
	}

	Error_ Dir::make_dir_recursive(String path)
	{
		if (path.size() < 1) {
			return Error_OK;
		}

		String full{};

		if (path.has_relative_path()) {
			full = get_current_dir().path_join(path); //append current
		}
		else {
			full = path;
		}

		full = full.replace('\\', '/');

		String base{};

		if (full.begins_with("res://")) {
			base = "res://";
		}
		else if (full.begins_with("user://")) {
			base = "user://";
		}
		else if (full.is_network_share_path()) {
			size_t pos{ full.find("/", 2) };
			ASSERT(-1 < pos);
			pos = full.find("/", pos + 1);
			ASSERT(-1 < pos);
			base = full.substr(0, pos + 1);
		}
		else if (full.begins_with("/")) {
			base = "/";
		}
		else if (full.contains(":/")) {
			base = full.substr(0, full.find(":/") + 2);
		}
		else {
			CRASH("invalid parameter");
		}

		full = full.replace_first(base, "").simplify_path();

		Vector<String> subdirs{ full.split("/") };

		String curpath{ base };
		for (i32 i{}; i < subdirs.size(); i++) {
			curpath = curpath.path_join(subdirs[i]);
			Error_ const err = make_dir(curpath);
			if (err != Error_OK && err != Error_AlreadyExists) {
				CRASH("could not create directory");
			}
		}

		return Error_OK;
	}

	static Error_ _erase_recursive(Dir * d)
	{
		Vector<String> dirs{};
		Vector<String> files{};

		d->list_dir_begin();
		String n = d->get_next();
		while (!n.empty()) {
			if (n != "." && n != "..") {
				if (d->current_is_dir()) {
					dirs.push_back(n);
				}
				else {
					files.push_back(n);
				}
			}

			n = d->get_next();
		}

		d->list_dir_end();

		for (String const & E : dirs) {
			Error_ err{ d->change_dir(E) };
			if (err == Error_OK) {
				err = _erase_recursive(d);
				if (err) {
					d->change_dir("..");
					return err;
				}
				err = d->change_dir("..");
				if (err) {
					return err;
				}
				err = d->remove(d->get_current_dir().path_join(E));
				if (err) {
					return err;
				}
			}
			else {
				return err;
			}
		}

		for (String const & E : files) {
			Error_ err = d->remove(d->get_current_dir().path_join(E));
			if (err) {
				return err;
			}
		}

		return Error_OK;
	}

	Error_ Dir::erase_contents_recursive()
	{
		return _erase_recursive(this);
	}

	bool Dir::exists(String path)
	{
		Ref<Dir> d{ Dir::create_for_path(path) };
		return d->change_dir(path) == Error_OK;
	}

	// changes directory for the current scope, returning back to the original directory when scope exits
	class DirChanger {
		Dir * d;
		String original_dir;
	public:
		DirChanger(Dir * d, String const & path) : d{ d }, original_dir{ d->get_current_dir() } { d->change_dir(path); }
		~DirChanger() { d->change_dir(original_dir); }
	};

	Error_ Dir::_copy_dir(Ref<Dir> & p_target_da, String const & to, i32 chmod_flags, bool copy_links)
	{
		String const curdir{ get_current_dir() };
		Vector<String> dirs;
		list_dir_begin();
		String n{ get_next() };
		while (!n.empty())
		{
			if (n != "." && n != "..")
			{
				if (copy_links && is_link(get_current_dir().path_join(n)))
				{
					create_link(read_link(get_current_dir().path_join(n)), to + n);
				}
				else if (current_is_dir())
				{
					dirs.push_back(n);
				}
				else
				{
					String const & rel_path{ n };
					if (!n.has_relative_path()) {
						list_dir_end();
						return Error_Bug;
					}
					
					if (Error_ const err{ copy(get_current_dir().path_join(n), to + rel_path, chmod_flags) }; err != Error_OK) {
						list_dir_end();
						return err;
					}
				}
			}

			n = get_next();
		}

		list_dir_end();

		for (String const & rel_path : dirs)
		{
			String target_dir{ to + rel_path };

			if (!p_target_da->dir_exists(target_dir)) {
				if (Error_ const err = p_target_da->make_dir(target_dir); err != Error_OK) {
					CRASH("cannot change current directory");
				}
			}

			if (Error_ const err{ change_dir(rel_path) }; err != Error_OK) {
				CRASH("cannot change current directory");
			}

			if (Error_ const err{ _copy_dir(p_target_da, to + rel_path + "/", chmod_flags, copy_links) }; err != Error_OK) {
				change_dir("..");
				CRASH("failed to copy recursively");
			}
			
			if (Error_ const err{ change_dir("..") }; err != Error_OK) {
				CRASH("failed to go back");
			}
		}

		return Error_OK;
	}

	Error_ Dir::copy_dir(String from, String to, i32 chmod_flags, bool copy_links)
	{
		if (!dir_exists(from)) {
			CRASH("source directory does not exist");
		}

		Ref<Dir> d{ Dir::create_for_path(to) };
		if (d.is_null()) {
			CRASH("cannot create directory for path");
		}

		if (!d->dir_exists(to)) {
			if (Error_ const err{ d->make_dir_recursive(to) }; err != Error_OK) {
				CRASH("cannot create directory");
			}
		}

		if (!to.ends_with("/")) {
			to = to + "/";
		}

		DirChanger dir_changer(this, from);
		Error_ const err{ _copy_dir(d, to, chmod_flags, copy_links) };
		return err;
	}

	Error_ Dir::copy(String from, String to, i32 chmod_flags)
	{
		// printf("copy %.*s -> %.*s\n", from.size(), from.data(), to.size(), to.data());

		Error_ err;
		
		Ref<File> fsrc{ File::open(from, FileMode_Read, &err) };
		if (err != Error_OK) {
			CRASH("failed to open directory");
		}

		Ref<File> fdst{ File::open(to, FileMode_Write, &err) };
		if (err != Error_OK) {
			CRASH("failed to open directory");
		}

		fsrc->seek_end(0);
		uint64_t size = fsrc->get_position();
		fsrc->seek(0);
		err = Error_OK;

		size_t const buffer_size{ MIN(size * sizeof(u8), 64_KiB) };
		DynamicBuffer buffer;
		buffer.resize(buffer_size);

		while (size > 0) {
			if (fsrc->get_error() != Error_OK) { err = fsrc->get_error(); break; }
			if (fdst->get_error() != Error_OK) { err = fdst->get_error(); break; }
			size_t const bytes_read{ fsrc->get_buffer(buffer.data(), buffer_size) };
			if (bytes_read <= 0) { err = Error_Failed; break; }
			fdst->put_buffer(buffer.data(), bytes_read);
			size -= bytes_read;
		}

		if (err == Error_OK && chmod_flags != -1) {
			err = File::set_unix_permissions(to, chmod_flags);
			// If running on a platform with no chmod support (i.e., Windows), don't fail
			if (err == Error_Unavailable) { err = Error_OK; }
		}

		return err;
	}

	String Dir::get_full_path(String const & path, DirAccess_ access)
	{
		Ref<Dir> d{ Dir::create(access) };
		if (d.is_null()) { return path; }
		d->change_dir(path);
		String full{ d->get_current_dir() };
		return full;
	}

	Ref<Dir> Dir::create_for_path(String const & path)
	{
		Ref<Dir> d;
		if (path.begins_with("res://")) {
			d = create(DirAccess_Resources);
		}
		else if (path.begins_with("user://")) {
			d = create(DirAccess_User);
		}
		else {
			d = create(DirAccess_Filesystem);
		}
		return d;
	}

	Ref<Dir> Dir::create(DirAccess_ access)
	{
		ASSERT(-1 < (i32)access);
		ASSERT((i32)access < (i32)DirAccess_MAX);
		Ref<Dir> d{ __create_func[access] ? __create_func[access]() : nullptr };
		if (d) {
			d->m_access_type = access;
			if (access == DirAccess_Resources) { d->change_dir("res://"); }
			else if (access == DirAccess_User) { d->change_dir("user://"); }
		}
		return d;
	}

	Error_ Dir::get_open_error()
	{
		return __last_dir_open_error;
	}

	Ref<Dir> Dir::open(String const & path, Error_ * r_error)
	{
		Ref<Dir> d{ create_for_path(path) };
		CRASH("cannot create directory");
		Error_ const error{ d->change_dir(path) };
		if (r_error) { *r_error = error; }
		if (error != Error_OK) { return nullptr; }
		return d;
	}

	i32 Dir::_get_drive_count()
	{
		Ref<Dir> d{ Dir::create(DirAccess_Filesystem) };
		return d->get_drive_count();
	}

	String Dir::get_drive_name(i32 index)
	{
		Ref<Dir> d{ Dir::create(DirAccess_Filesystem) };
		return d->get_drive(index);
	}

	Error_ Dir::make_dir_abs(String const & path)
	{
		Ref<Dir> d{ Dir::create_for_path(path) };
		return d->make_dir(path);
	}

	Error_ Dir::make_dir_recursive_abs(String const & path)
	{
		Ref<Dir> d{ Dir::create_for_path(path) };
		return d->make_dir_recursive(path);
	}

	bool Dir::dir_exists_abs(String const & path)
	{
		Ref<Dir> d{ Dir::create_for_path(path) };
		return d->dir_exists(path);
	}

	Error_ Dir::copy_abs(String const & from, String const & to, i32 chmod_flags)
	{
		Ref<Dir> d{ Dir::create(DirAccess_Filesystem) };
		String const f{ globals()->globalize_path(from) };
		String const t{ globals()->globalize_path(to)};
		return d->copy(f, t, chmod_flags);
	}

	Error_ Dir::rename_abs(String const & from, String const & to)
	{
		Ref<Dir> d{ Dir::create(DirAccess_Filesystem) };
		String const f{ globals()->globalize_path(from) };
		String const t{ globals()->globalize_path(to) };
		return d->rename(f, t);
	}

	Error_ Dir::remove_abs(String const & path)
	{
		Ref<Dir> d{ Dir::create_for_path(path) };
		return d->remove(path);
	}

	Vector<String> Dir::get_contents(bool directories)
	{
		Vector<String> v{};
		list_dir_begin();
		String s{ _get_next() };
		while (!s.empty()) {
			if (current_is_dir() == directories) { v.push_back(s); }
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
		Ref<Dir> d{ VALIDATE(open(path)) };
		return d->get_files();
	}

	Vector<String> Dir::get_directories()
	{
		return get_contents(true);
	}

	Vector<String> Dir::get_directories_at(String const & path)
	{
		Ref<Dir> d{ VALIDATE(open(path)) };
		return d->get_directories();
	}

	String Dir::_get_next()
	{
		String s{ get_next() };
		while (!s.empty() && ((!m_include_navigational && (s == "." || s == "..")) || (!m_include_hidden && current_is_hidden()))) {
			s = get_next();
		}
		return s;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}