#include <drivers/windows/windows_dir.hpp>
#include <core/os/os.hpp>
#include <core/config/project_settings.hpp>
#include <platform/windows/windows.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void WindowsDir::initialize()
	{
		Dir::__create_func[DirAccess_Filesystem] = []() -> Ref<Dir> { return memnew(WindowsDir); };
		Dir::__create_func[DirAccess_Resources] = []() -> Ref<Dir> { return memnew(WindowsDir); };
		Dir::__create_func[DirAccess_User] = []() -> Ref<Dir> { return memnew(WindowsDir); };
	}

	void WindowsDir::finalize()
	{
		Dir::__create_func[DirAccess_Filesystem] = nullptr;
		Dir::__create_func[DirAccess_Resources] = nullptr;
		Dir::__create_func[DirAccess_User] = nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct WindowsDirPrivate
	{
		HANDLE h; //handle for findfirstfile
		WIN32_FIND_DATA f;
		WIN32_FIND_DATAW fu; //unicode version
	};

	Error_ WindowsDir::list_dir_begin()
	{
		m_cisdir = false;
		m_cishidden = false;

		list_dir_end();
		m_ptr->h = FindFirstFileExW((LPCWSTR)(String(m_path + "\\*").widen().c_str()), FindExInfoStandard, &m_ptr->fu, FindExSearchNameMatch, nullptr, 0);

		if (m_ptr->h == INVALID_HANDLE_VALUE) {
			return Error_Failed;
		}

		return Error_OK;
	}

	String WindowsDir::get_next()
	{
		if (m_ptr->h == INVALID_HANDLE_VALUE) {
			return "";
		}

		m_cisdir = (m_ptr->fu.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
		m_cishidden = (m_ptr->fu.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN);

		String name = Unicode(m_ptr->fu.cFileName).narrow();

		if (FindNextFileW(m_ptr->h, &m_ptr->fu) == 0) {
			FindClose(m_ptr->h);
			m_ptr->h = INVALID_HANDLE_VALUE;
		}

		return name;
	}

	bool WindowsDir::current_is_dir() const
	{
		return m_cisdir;
	}

	bool WindowsDir::current_is_hidden() const
	{
		return m_cishidden;
	}

	void WindowsDir::list_dir_end()
	{
		if (m_ptr->h != INVALID_HANDLE_VALUE) {
			FindClose(m_ptr->h);
			m_ptr->h = INVALID_HANDLE_VALUE;
		}
	}

	i32 WindowsDir::get_drive_count()
	{
		return m_drive_count;
	}

	String WindowsDir::get_drive(i32 drive)
	{
		return (drive < 0 || drive >= m_drive_count) ? "" : String{} + m_drives[drive] + ':';
	}

	Error_ WindowsDir::change_dir(String path)
	{
		path = fix_path(path);

		WCHAR real_current_dir_name[2048];
		GetCurrentDirectoryW(2048, real_current_dir_name);
		String prev_dir = Unicode(real_current_dir_name).narrow();

		SetCurrentDirectoryW((LPCWSTR)(m_path.widen().c_str()));
		bool worked = (SetCurrentDirectoryW((LPCWSTR)path.widen().c_str()) != 0);

		String base = _get_root_path();
		if (!base.empty()) {
			GetCurrentDirectoryW(2048, real_current_dir_name);
			String new_dir = Unicode(real_current_dir_name).narrow().replace("\\", "/");
			if (!new_dir.begins_with(base)) {
				worked = false;
			}
		}

		if (worked) {
			GetCurrentDirectoryW(2048, real_current_dir_name);
			m_path = Unicode(real_current_dir_name).narrow();
			m_path = m_path.replace("\\", "/");
		}

		SetCurrentDirectoryW((LPCWSTR)(prev_dir.widen().c_str()));

		return worked ? Error_OK : Error_Failed;
	}

	Error_ WindowsDir::make_dir(String path)
	{
		path = fix_path(path);
		if (path.has_relative_path()) {
			path = m_path.path_join(path);
		}

		path = path.simplify_path().replace("/", "\\");

		bool success;
		i32 err;

		success = CreateDirectoryW((LPCWSTR)path.widen().c_str(), nullptr);
		err = GetLastError();

		if (success) {
			return Error_OK;
		}

		if (err == ERROR_ALREADY_EXISTS || err == ERROR_ACCESS_DENIED) {
			return Error_Failed;
		}

		return Error_Failed;
	}

	String WindowsDir::get_current_dir(bool include_drive) const
	{
		String base = _get_root_path();
		if (!base.empty()) {
			String bd = m_path.replace("\\", "/");
			if (bd.begins_with("/")) {
				return _get_root_string() + bd.substr(1, bd.size());
			}
			else {
				return _get_root_string() + bd;
			}
		}

		if (include_drive) {
			return m_path;
		}
		else {
			if (_get_root_string().empty()) {
				size_t pos = m_path.find(":");
				if (pos != String::npos) {
					return m_path.substr(pos + 1);
				}
			}
			return m_path;
		}
	}

	bool WindowsDir::file_exists(String path)
	{
		//if (!path.is_abs_path()) {
		//	path = get_current_dir().path_join(path);
		//}

		path = fix_path(path);

		DWORD fileAttr;

		fileAttr = GetFileAttributesW((LPCWSTR)path.widen().c_str());
		if (INVALID_FILE_ATTRIBUTES == fileAttr) {
			return false;
		}

		return !(fileAttr & FILE_ATTRIBUTE_DIRECTORY);
	}

	bool WindowsDir::dir_exists(String path)
	{
		if (path.has_relative_path()) {
			path = get_current_dir().path_join(path);
		}

		path = fix_path(path);

		DWORD fileAttr;
		fileAttr = GetFileAttributesW((LPCWSTR)path.widen().c_str());
		if (INVALID_FILE_ATTRIBUTES == fileAttr) {
			return false;
		}
		return (fileAttr & FILE_ATTRIBUTE_DIRECTORY);
	}

	Error_ WindowsDir::rename(String path, String new_path)
	{
		if (path.has_relative_path()) {
			path = get_current_dir().path_join(path);
		}

		path = fix_path(path);

		if (new_path.has_relative_path()) {
			new_path = get_current_dir().path_join(new_path);
		}

		new_path = fix_path(new_path);

		// If we're only changing file name case we need to do a little juggling
		if (path.lowercase() == new_path.lowercase()) {
			if (dir_exists(path)) {
				// The path is a dir; just rename
				return ::_wrename((LPCWSTR)path.widen().c_str(), (LPCWSTR)new_path.widen().c_str()) == 0 ? Error_OK : Error_Failed;
			}
			// The path is a file; juggle
			WCHAR tmpfile[MAX_PATH];

			if (!::GetTempFileNameW((LPCWSTR)fix_path(get_current_dir()).widen().c_str(), nullptr, 0, tmpfile)) {
				return Error_Failed;
			}

			if (!::ReplaceFileW(tmpfile, (LPCWSTR)path.widen().c_str(), nullptr, 0, nullptr, nullptr)) {
				DeleteFileW(tmpfile);
				return Error_Failed;
			}

			return ::_wrename(tmpfile, (LPCWSTR)new_path.widen().c_str()) == 0 ? Error_OK : Error_Failed;

		}
		else
		{
			if (file_exists(new_path)) {
				if (remove(new_path) != Error_OK) {
					return Error_Failed;
				}
			}
			return ::_wrename((LPCWSTR)path.widen().c_str(), (LPCWSTR)new_path.widen().c_str()) == 0 ? Error_OK : Error_Failed;
		}
	}

	Error_ WindowsDir::remove(String path)
	{
		if (path.has_relative_path()) {
			path = get_current_dir().path_join(path);
		}

		path = fix_path(path);

		DWORD fileAttr;

		fileAttr = GetFileAttributesW((LPCWSTR)path.widen().c_str());
		if (INVALID_FILE_ATTRIBUTES == fileAttr) {
			return Error_Failed;
		}
		if ((fileAttr & FILE_ATTRIBUTE_DIRECTORY)) {
			return ::_wrmdir((LPCWSTR)path.widen().c_str()) == 0 ? Error_OK : Error_Failed;
		}
		else {
			return ::_wunlink((LPCWSTR)path.widen().c_str()) == 0 ? Error_OK : Error_Failed;
		}
	}

	u64 WindowsDir::get_space_left()
	{
		u64 bytes;
		return GetDiskFreeSpaceEx(nullptr, (PULARGE_INTEGER)&bytes, nullptr, nullptr) ? bytes : 0;
	}

	String WindowsDir::get_filesystem_type() const
	{
		return String();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(WindowsDir, t) {}

	String WindowsDir::fix_path(String path) const {
		String r_path{ Dir::fix_path(path) };
		if (r_path.is_absolute_path() && !r_path.is_network_share_path() && r_path.size() > MAX_PATH) {
			r_path = "\\\\?\\" + r_path.replace("/", "\\");
		}
		return r_path;
	}

	WindowsDir::WindowsDir()
	{
		m_ptr = memnew(WindowsDirPrivate);
		m_ptr->h = INVALID_HANDLE_VALUE;
		m_path = ".";

		DWORD mask = GetLogicalDrives();

		for (int i = 0; i < MAX_DRIVES; i++) {
			if (mask & (1 << i)) { //DRIVE EXISTS

				m_drives[m_drive_count] = 'A' + i;
				m_drive_count++;
			}
		}

		change_dir(".");
	}

	WindowsDir::~WindowsDir()
	{
		list_dir_end();

		memdelete(m_ptr);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}