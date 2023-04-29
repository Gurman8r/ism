#ifndef _ISM_FILE_ACCESS_ZIP_HPP_
#define _ISM_FILE_ACCESS_ZIP_HPP_

#include <core/io/file_access_pack.hpp>

#include <minizip/unzip.h>

namespace ism
{
	class ISM_API ZipArchive : public PackSource
	{
	public:
		struct File {
			i32 package{ -1 };
			unz_file_pos file_pos{};
		};

		struct Package {
			String path{};
			unzFile m_zfile{};
		};
		
	private:
		static ZipArchive * __singleton;
		Vector<Package> m_packages{};
		HashMap<String, File> m_files{};

	public:
		ZipArchive();
		~ZipArchive();
		static ZipArchive * get_singleton();

		void close_handle(unzFile file) const;
		NODISCARD unzFile get_file_handle(String const & path) const;
		NODISCARD bool file_exists(String const & path) const;
		NODISCARD virtual bool try_open_pack(String const & path, bool replace_files, u64 offset) override;
		NODISCARD virtual Ref<FileAccess> get_file(String const & path, Packages::PackedFile * file) override;
	};
}

namespace ism
{
	class ISM_API FileAccessZip : public FileAccess
	{
		DEFINE_CLASS(FileAccessZip, FileAccess);

		unzFile				m_zfile{};
		unz_file_info64		m_info{};
		mutable bool		m_eof{};

	public:
		FileAccessZip(String const & path, Packages::PackedFile const & file);
		virtual ~FileAccessZip() override;

		virtual Error_ open_internal(String const & path, FileMode_ mode) override;
		virtual void close() override;
		virtual void flush() override;
		virtual bool exists(String const & path) override;
		virtual bool is_open() const override;
		virtual void seek(u64 position) override;
		virtual void seek_end(i64 position) override;
		virtual u64 get_position() const override;
		virtual u64 get_length() const override;
		virtual bool eof_reached() const override;
		virtual Error_ get_error() const override;
		virtual u8 read_8() const override;
		virtual size_t read_buffer(u8 * data, size_t const size) const override;
		virtual void write_8(u8) override;
	};
}

#endif // !_ISM_FILE_ACCESS_ZIP_HPP_
