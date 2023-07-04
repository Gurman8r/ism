#include <drivers/windows/windows_file.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void WindowsFile::_check_errors() const
	{
		ASSERT(m_file);
		if (feof(m_file)) {
			m_last_error = Error_EOF;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(WindowsFile, t) {}

	WindowsFile::~WindowsFile() noexcept { WindowsFile::close(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	String WindowsFile::fix_path(String const & path) const
	{
		return String();
	}

	Error_ WindowsFile::open_internal(String const & path, FileMode_ mode)
	{
		WindowsFile::close();
		constexpr cstring m_mode[4]{ "rb", "wb", "rb+", "wb+" };
		m_file = fopen(path.c_str(), m_mode[mode]);
		if (!m_file) { return Error_Failed; }
		return Error_OK;
	}

	u64 WindowsFile::_get_modified_time(String const & path)
	{
		// if (is_path_invalid(path)) { return 0; }

		String file{ fix_path(path) };
		if (file.has_suffix("/") && file != "/") { file = file.substr(0, file.size() - 1); }

		struct _stat st;

		i32 rv{ _wstat(file.widen().data(), &st) };

		if (rv == 0) { return st.st_mtime;
		}
		else {
			PRINT_LINE("Failed to get modified time for: " + path + "");
			return 0;
		}
	}

	u32 WindowsFile::_get_unix_permissions(String const & path)
	{
		return 0;
	}

	Error_ WindowsFile::_set_unix_permissions(String const & path, u32 permissions)
	{
		return Error_Unavailable;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	WindowsFile & WindowsFile::flush()
	{
		ASSERT(m_file);
		fflush(m_file);
		if (m_last_op == FileMode_Write) {
			m_last_op = FileMode_MAX;
		}
		return (*this);
	}

	WindowsFile & WindowsFile::close()
	{
		if (m_file) {
			fclose(m_file);
			m_file = nullptr;
		}
		return (*this);
	}

	bool WindowsFile::file_exists(String const & path)
	{
		FILE * const fp{ fopen(path.c_str(), "rb") };
		ON_SCOPE_EXIT(&fp) { fclose(fp); };
		return fp != nullptr;
	}

	bool WindowsFile::is_open() const
	{
		return m_file != nullptr;
	}

	String WindowsFile::get_path() const
	{
		return m_path;
	}

	String WindowsFile::get_path_abs() const
	{
		return m_path_abs;
	}

	WindowsFile & WindowsFile::seek(u64 position)
	{
		ASSERT(m_file);
		m_last_error = Error_OK;
		if (_fseeki64(m_file, position, SEEK_SET)) {
			_check_errors();
		}
		m_last_op = FileMode_MAX;
		return (*this);
	}

	WindowsFile & WindowsFile::seek_end(i64 position)
	{
		ASSERT(m_file);
		m_last_error = Error_OK;
		if (_fseeki64(m_file, position, SEEK_END)) {
			_check_errors();
		}
		m_last_op = FileMode_MAX;
		return (*this);
	}

	u64 WindowsFile::get_position() const
	{
		i64 const pos{ _ftelli64(m_file) };
		if (pos < 0) {
			_check_errors();
		}
		return pos;
	}

	u64 WindowsFile::get_length() const
	{
		ASSERT(m_file);
		u64 const pos{ get_position() };
		_fseeki64(m_file, 0, SEEK_END);
		u64 const size{ get_position() };
		_fseeki64(m_file, pos, SEEK_SET);
		return size;
	}

	bool WindowsFile::eof_reached() const
	{
		_check_errors();
		return m_last_error == Error_EOF;
	}

	Error_ WindowsFile::get_error() const
	{
		return m_last_error;
	}

	u8 WindowsFile::get_8() const
	{
		ASSERT(m_file);
		if (m_mode == FileMode_Write || m_mode == FileMode_Read) {
			if (m_last_op == FileMode_Write) {
				fflush(m_file);
			}
			m_last_op = FileMode_Read;
		}
		u8 b;
		if (fread(&b, 1, 1, m_file) == 0) {
			_check_errors();
			b = '\0';
		}
		return b;
	}

	WindowsFile & WindowsFile::put_8(u8 value)
	{
		ASSERT(m_file);
		if (m_mode == FileMode_ReadWrite || m_mode == FileMode_WriteRead) {
			if (m_last_op == FileMode_Read) {
				if (m_last_error != Error_EOF) {
					fseek(m_file, 0, SEEK_CUR);
				}
			}
			m_last_op = FileMode_Write;
		}
		fwrite(&value, 1, 1, m_file);
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void WindowsFile::initialize()
	{
		File::__create_func[FileAccess_Resources] = []() -> Ref<File> { return memnew(WindowsFile); };
		File::__create_func[FileAccess_User] = []() -> Ref<File> { return memnew(WindowsFile); };
		File::__create_func[FileAccess_Filesystem] = []() -> Ref<File> { return memnew(WindowsFile); };
	}

	void WindowsFile::finalize()
	{
		File::__create_func[FileAccess_Resources] = nullptr;
		File::__create_func[FileAccess_User] = nullptr;
		File::__create_func[FileAccess_Filesystem] = nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}