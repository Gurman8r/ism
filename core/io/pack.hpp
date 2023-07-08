#ifndef _ISM_PACK_HPP_
#define _ISM_PACK_HPP_

#include <core/io/dir.hpp>
#include <core/io/file.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum PackageDirFlags_ { PackageDirFlags_Encrypted = 1 << 0 };

	enum PackageFileFlags_ { PackageFileFlags_Encrypted = 1 << 0 };

	class PackSource;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// package manager
	class ISM_API PackedData
	{
	public:
		using PathID = size_t;

		struct PackedFile {
			String pack{};
			u64 offset{}; //if offset is ZERO, the file was ERASED
			u64 size{};
			PathID id{};
			PackSource * src{};
			bool encrypted{};
		};

		struct PackedDir {
			PackedDir * parent{};
			String name{};
			HashMap<String, PackedDir *> subdirs{};
			HashSet<String> files{};
			~PackedDir() { for (auto & [k, v] : subdirs) { memdelete(v); } }
		};

	private:
		static PackedData * __singleton;
		bool m_enabled{ true };
		HashMap<String, PackedFile> m_files{};
		Vector<PackSource *> m_sources{};
		PackedDir * m_root{};

	public:
		PackedData();
		~PackedData();
		SINGLETON_GETTER(PackedData, __singleton);

		NODISCARD bool is_enabled() const { return m_enabled; }
		void set_enabled(bool enabled) { m_enabled = enabled; }

		Error_ add_package(String const & path, bool replace_files, u64 offset);
		void add_package_source(PackSource * source);
		void add_path(String const & package_path, String const & path, u64 offset, u64 size, PathID id, PackSource * src, bool replace_files, bool encrypted = false);

		NODISCARD Ref<File> try_open_path(String const & path);
		NODISCARD bool has_path(String const & path);

		NODISCARD Ref<Dir> try_open_dir(String const & path);
		NODISCARD bool has_dir(String const & path);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// pack source
	class ISM_API PackSource {
	public:
		virtual ~PackSource() noexcept = default;
		virtual bool try_open_pack(String const & path, bool replace_files, u64 offset) = 0;
		virtual Ref<File> get_file(String const & path, PackedData::PackedFile * file) = 0;
	};

	// pack source PAK
	class ISM_API PackSourcePAK : public PackSource {
	public:
		virtual bool try_open_pack(String const & path, bool replace_files, u64 offset) override;
		virtual Ref<File> get_file(String const & path, PackedData::PackedFile * file) override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// pack file
	class ISM_API PackFile : public File
	{
		DEFINE_CLASS(PackFile, File);

		PackedData::PackedFile m_pf{};
		mutable u64 m_pos{};
		mutable bool m_eof{};
		u64 m_off{};
		Ref<File> m_f{};

	protected:
		virtual Error_ open_internal(String const & path, FileMode_ mode) override;
		virtual u64 _get_modified_time(String const & path) override { return 0; }
		virtual u32 _get_unix_permissions(String const & path) override { return 0; }
		virtual Error_ _set_unix_permissions(String const & path, u32 permissions) override { return Error_Failed; }

	public:
		PackFile(String const & path, PackedData::PackedFile const & file);
		virtual bool is_open() const override;
		virtual PackFile & seek(u64 position) override;
		virtual PackFile & seek_end(int64_t position = 0) override;
		virtual u64 get_position() const override;
		virtual u64 get_length() const override;
		virtual bool eof_reached() const override;
		virtual u8 get_8() const override;
		virtual u64 get_buffer(u8 * data, u64 size) const override;
		virtual void set_big_endian(bool big_endian) noexcept override;
		virtual Error_ get_error() const override;
		virtual PackFile & flush() override;
		virtual PackFile & put_8(u8 p_dest) override;
		virtual PackFile & put_buffer(u8 const * data, u64 size) override;
		virtual bool file_exists(String const & name) override;
		virtual PackFile & close() override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// pack dir
	class ISM_API PackDir : public Dir
	{
		DEFINE_CLASS(PackDir, Dir);

		PackedData::PackedDir * m_current{};
		Vector<String> m_list_dirs{};
		Vector<String> m_list_files{};
		bool m_cdir{};

		PackedData::PackedDir * _find_dir(String path);

	public:
		PackDir();
		virtual Error_ list_dir_begin() override;
		virtual String get_next() override;
		virtual bool current_is_dir() const override;
		virtual bool current_is_hidden() const override;
		virtual void list_dir_end() override;
		virtual i32 get_drive_count() override;
		virtual String get_drive(i32 drive) override;
		virtual Error_ change_dir(String path) override;
		virtual String get_current_dir(bool include_drive = true) const override;
		virtual bool file_exists(String path) override;
		virtual bool dir_exists(String path) override;
		virtual Error_ make_dir(String path) override;
		virtual Error_ rename(String from, String to) override;
		virtual Error_ remove(String p_name) override;
		u64 get_space_left() override;
		virtual bool is_link(String path) override { return false; }
		virtual String read_link(String path) override { return path; }
		virtual Error_ create_link(String source, String target) override { return Error_Failed; }
		virtual String get_filesystem_type() const override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_PACKAGE_FILE_HPP_
