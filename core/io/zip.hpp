#ifndef _ISM_ZIP_HPP_
#define _ISM_ZIP_HPP_

#include <core/io/pack.hpp>

#include <minizip/unzip.h>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// zip archive
	class ISM_API ZipArchive : public PackSource
	{
	public:
		struct ZipFile {
			i32 package{ -1 };
			unz_file_pos file_pos{};
		};

		struct ZipPackage {
			String path{};
			unzFile m_zfile{};
		};
		
	private:
		static ZipArchive * __singleton;
		Vector<ZipPackage> m_packages{};
		HashMap<String, ZipFile> m_files{};

	public:
		ZipArchive();
		~ZipArchive();
		static ZipArchive * get_singleton();

		void close_handle(unzFile file) const;
		NODISCARD unzFile get_file_handle(String const & path) const;
		NODISCARD bool file_exists(String const & path) const;
		NODISCARD virtual bool try_open_pack(String const & path, bool replace_files, u64 offset) override;
		NODISCARD virtual Ref<File> get_file(String const & path, PackedData::PackedFile * file) override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// zipped file
	class ISM_API ZippedFile : public File
	{
		OBJECT_CLASS(ZippedFile, File);

		unzFile			m_zfile{};
		unz_file_info64	m_info{};
		mutable bool	m_eof{};

	public:
		ZippedFile(String const & path, PackedData::PackedFile const & file);
		virtual ~ZippedFile() override;

	protected:
		virtual Error_ open_internal(String const & path, FileMode_ mode) override;
		virtual u64 _get_modified_time(String const & path) override { return 0; }
		virtual u32 _get_unix_permissions(String const & path) override { return 0; }
		virtual Error_ _set_unix_permissions(String const & path, u32 permissions) override { return Error_Failed; }

	public:
		virtual ZippedFile & close() override;
		virtual ZippedFile & flush() override;
		virtual bool file_exists(String const & path) override;
		virtual bool is_open() const override;
		virtual ZippedFile & seek(u64 position) override;
		virtual ZippedFile & seek_end(i64 position) override;
		virtual u64 get_position() const override;
		virtual u64 get_length() const override;
		virtual bool eof_reached() const override;
		virtual Error_ get_error() const override;
		virtual String get_path() const override;
		virtual String get_path_abs() const override;
		virtual u8 get_8() const override;
		virtual size_t get_buffer(u8 * data, size_t const size) const override;
		virtual ZippedFile & put_8(u8) override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_ZIP_HPP_
