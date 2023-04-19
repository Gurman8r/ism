#include <drivers/windows/file_access_windows.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(FileAccessWindows, t) {}

	FileAccessWindows::~FileAccessWindows() noexcept { _close(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void FileAccessWindows::_check_errors() const
	{
		ASSERT(m_file);
		if (feof(m_file)) {
			m_last_error = Error_EOF;
		}
	}

	void FileAccessWindows::_close()
	{
		if (!m_file) { return; }
		fclose(m_file);
		m_file = nullptr;
	}

	Error_ FileAccessWindows::open_internal(Path const & path, FileMode_ mode)
	{
		_close();

		constexpr cstring m_mode[4]{ "rb", "wb", "rb+", "wb+" };

		m_file = fopen(path.c_str(), m_mode[mode]);

		if (!m_file) {
			return Error_Unknown;
		}

		return Error_OK;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void FileAccessWindows::close()
	{
		_close();
	}

	void FileAccessWindows::flush()
	{
		ASSERT(m_file);
		fflush(m_file);
		if (m_last_op == FileMode_Write) {
			m_last_op = FileMode_MAX;
		}
	}

	bool FileAccessWindows::exists(Path const & path)
	{
		FILE * const fp{ fopen(path.c_str(), "rb") };
		ON_SCOPE_EXIT(&fp) { fclose(fp); };
		return (bool)fp;
	}

	bool FileAccessWindows::is_open() const
	{
		return m_file != nullptr;
	}

	Path FileAccessWindows::get_path() const
	{
		return m_path;
	}

	Path FileAccessWindows::get_path_absolute() const
	{
		return m_path_abs;
	}

	void FileAccessWindows::seek(u64 position)
	{
		ASSERT(m_file);
		m_last_error = Error_OK;
		if (_fseeki64(m_file, position, SEEK_SET)) {
			_check_errors();
		}
		m_last_op = FileMode_MAX;
	}

	void FileAccessWindows::seek_end(i64 position)
	{
		ASSERT(m_file);
		m_last_error = Error_OK;
		if (_fseeki64(m_file, position, SEEK_END)) {
			_check_errors();
		}
		m_last_op = FileMode_MAX;
	}

	u64 FileAccessWindows::get_position() const
	{
		i64 const pos{ _ftelli64(m_file) };
		if (pos < 0) {
			_check_errors();
		}
		return pos;
	}

	u64 FileAccessWindows::get_length() const
	{
		ASSERT(m_file);
		u64 const pos{ get_position() };
		_fseeki64(m_file, 0, SEEK_END);
		u64 const size{ get_position() };
		_fseeki64(m_file, pos, SEEK_SET);
		return size;
	}

	bool FileAccessWindows::eof_reached() const
	{
		_check_errors();
		return m_last_error == Error_EOF;
	}

	Error_ FileAccessWindows::get_error() const
	{
		return m_last_error;
	}

	u8 FileAccessWindows::read_8() const
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

	void FileAccessWindows::write_8(u8 value)
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
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void FileAccessWindows::initialize()
	{
		FileAccess::__create_func[FileAccessType_Resources] = []() -> Ref<FileAccess> { return memnew(FileAccessWindows); };
		FileAccess::__create_func[FileAccessType_UserData] = []() -> Ref<FileAccess> { return memnew(FileAccessWindows); };
		FileAccess::__create_func[FileAccessType_Filesystem] = []() -> Ref<FileAccess> { return memnew(FileAccessWindows); };
	}

	void FileAccessWindows::finalize()
	{
		FileAccess::__create_func[FileAccessType_Resources] = nullptr;
		FileAccess::__create_func[FileAccessType_UserData] = nullptr;
		FileAccess::__create_func[FileAccessType_Filesystem] = nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}