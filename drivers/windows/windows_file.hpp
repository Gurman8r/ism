#ifndef _ISM_WINDOWS_FILE_HPP_
#define _ISM_WINDOWS_FILE_HPP_

#include <core/io/file.hpp>

#include <stdio.h>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API WindowsFile : public File
	{
		OBJECT_CLASS(WindowsFile, File);

		FILE * m_file{};
		FileMode_ m_mode{};
		mutable FileMode_ m_last_op{};
		mutable Error_ m_last_error{};
		String m_path{}, m_path_abs{}, m_path_save{};

		void _check_errors() const;

	public:
		WindowsFile() noexcept = default;
		virtual ~WindowsFile() noexcept override;

	protected:
		virtual String fix_path(String const & path) const override;
		virtual Error_ open_internal(String const & path, FileMode_ mode) override;
		virtual u64 _get_modified_time(String const & path) override;
		virtual u32 _get_unix_permissions(String const & path) override;
		virtual Error_ _set_unix_permissions(String const & path, u32 permissions) override;

	public:
		virtual WindowsFile & flush() override;
		virtual WindowsFile & close() override;
		virtual bool file_exists(String const & path) override;
		virtual bool is_open() const override;
		virtual String get_path() const override;
		virtual String get_path_abs() const override;
		virtual WindowsFile & seek(u64 position) override;
		virtual WindowsFile & seek_end(i64 position) override;
		virtual u64 get_position() const override;
		virtual u64 get_length() const override;
		virtual bool eof_reached() const override;
		virtual Error_ get_error() const override;
		virtual u8 get_8() const override;
		virtual WindowsFile & put_8(u8 value) override;

		static void initialize();
		static void finalize();
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_WINDOWS_FILE_HPP_
