#ifndef _ISM_FILE_ACCESS_WINDOWS_HPP_
#define _ISM_FILE_ACCESS_WINDOWS_HPP_

#include <core/io/file_access.hpp>

#include <stdio.h>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API FileAccessWindows : public FileAccess
	{
		DEFINE_CLASS(FileAccessWindows, FileAccess);

		FILE * m_file{};
		FileMode_ m_mode{};
		mutable FileMode_ m_last_op{};
		mutable Error_ m_last_error{};
		String m_path{}, m_path_abs{}, m_path_save{};

		void _check_errors() const;
		void _close();

	public:
		FileAccessWindows() noexcept {}
		virtual ~FileAccessWindows() noexcept override;

		virtual Error_ open_internal(String const & path, FileMode_ mode) override;
		virtual void flush() override;
		virtual void close() override;
		virtual bool exists(String const & path) override;
		virtual bool is_open() const override;
		virtual String get_path() const override;
		virtual String get_path_absolute() const override;
		virtual void seek(u64 position) override;
		virtual void seek_end(i64 position) override;
		virtual u64 get_position() const override;
		virtual u64 get_length() const override;
		virtual bool eof_reached() const override;
		virtual Error_ get_error() const override;
		virtual u8 read_8() const override;
		virtual void write_8(u8 value) override;

		static void initialize();
		static void finalize();
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_FILE_ACCESS_WINDOWS_HPP_
