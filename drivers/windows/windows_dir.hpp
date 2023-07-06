#ifndef _ISM_WINDOWS_DIR_HPP_
#define _ISM_WINDOWS_DIR_HPP_

#include <core/io/dir.hpp>

namespace Ism
{
	struct WindowsDirPrivate;

	class ISM_API WindowsDir : public Dir
	{
		DEFINE_CLASS(WindowsDir, Dir);

		enum { MAX_DRIVES = 26 };

		WindowsDirPrivate * m_ptr{};
		char m_drives[MAX_DRIVES]{};
		i32 m_drive_count{};
		String m_path{};
		bool m_cisdir{};
		bool m_cishidden{};

	protected:
		virtual String fix_path(String path) const override;

	public:
		WindowsDir();
		virtual ~WindowsDir() override;

		static void initialize();
		static void finalize();

	public:
		virtual Error_ list_dir_begin() override; ///< This starts dir listing
		virtual String get_next() override;
		virtual bool current_is_dir() const override;
		virtual bool current_is_hidden() const override;
		virtual void list_dir_end() override; ///<

		virtual i32 get_drive_count() override;
		virtual String get_drive(i32 drive) override;

		virtual Error_ change_dir(String path) override; ///< can be relative or abs, return false on success
		virtual String get_current_dir(bool include_drive = true) const override; ///< return current dir location

		virtual bool file_exists(String path) override;
		virtual bool dir_exists(String path) override;

		virtual Error_ make_dir(String path) override;

		virtual Error_ rename(String path, String new_path) override;
		virtual Error_ remove(String path) override;

		virtual bool is_link(String path) override { return false; };
		virtual String read_link(String path) override { return path; };
		virtual Error_ create_link(String src, String dst) override { return Error_Failed; };

		u64 get_space_left() override;

		virtual String get_filesystem_type() const override;
	};
}

#endif // !_ISM_WINDOWS_DIR_HPP_
