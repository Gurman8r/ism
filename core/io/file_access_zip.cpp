#include <core/io/file_access_zip.hpp>

namespace ism
{
	struct ZipData { Ref<FileAccess> f{}; };

	static void * ism_open(void * opaque, cstring path, i32 mode) {
		if (mode & ZLIB_FILEFUNC_MODE_WRITE) {
			return nullptr;
		}
		Ref<FileAccess> f{ FileAccess::open(path, FileMode_Read) };
		ASSERT(f);
		ZipData * zd{ memnew(ZipData) };
		zd->f = f;
		return zd;
	}

	static u64 ism_read(void * opaque, void * stream, void * buf, u64 size) {
		ZipData * zd{ (ZipData *)stream };
		zd->f->read_buffer((u8 *)buf, size);
		return size;
	}

	static u64 ism_write(void * opaque, void * stream, void const * buf, u64 size) {
		return 0;
	}

	static i64 ism_tell(void * opaque, void * stream) {
		ZipData * zd{ (ZipData *)stream };
		return zd->f->get_position();
	}

	static i64 ism_seek(void * opaque, void * stream, u64 offset, i32 origin) {
		ZipData * zd{ (ZipData *)stream };
		u64 pos{ offset };
		switch (origin) {
		default: break;
		case ZLIB_FILEFUNC_SEEK_CUR: { pos = zd->f->get_position() + offset; } break;
		case ZLIB_FILEFUNC_SEEK_END: { pos = zd->f->get_length() + offset; } break;
		};
		zd->f->seek(pos);
		return 0;
	}

	static i32 ism_close(void * opaque, void * stream) {
		ZipData * zd{ (ZipData *)stream };
		memdelete(zd);
		return 0;
	}

	static i32 ism_testerror(void * opaque, void * stream) {
		ZipData * zd{ (ZipData *)stream };
		return zd->f->get_error() != Error_OK ? 1 : 0;
	}

	static void * ism_alloc(void * opaque, u64 items, u64 size) {
		return memalloc((size_t)items * size);
	}

	static void ism_free(void * opaque, void * address) {
		memfree(address);
	}
}

namespace ism
{
	ZipArchive * ZipArchive::__singleton{};

	ZipArchive::ZipArchive()
	{
		__singleton = this;
	}

	ZipArchive::~ZipArchive()
	{
		for (size_t i{}; i < m_packages.size(); ++i) {
			unzClose(m_packages[i].m_zfile);
		}
		m_packages.clear();
	}

	ZipArchive * ZipArchive::get_singleton()
	{
		return BRANCHLESS_IF((!__singleton), (__singleton = memnew(ZipArchive))), __singleton;
	}

	void ZipArchive::close_handle(unzFile file) const
	{
		ASSERT(file);
		unzCloseCurrentFile(file);
		unzClose(file);
	}

	unzFile ZipArchive::get_file_handle(Path const & path) const
	{
		ASSERT(file_exists(path));
		File file{ *util::getptr(m_files, path) };

		zlib_filefunc_def io;
		memset(&io, 0, sizeof(io));
		io.opaque = nullptr;
		io.zopen_file = (open_file_func)ism_open;
		io.zread_file = (read_file_func)ism_read;
		io.zwrite_file = (write_file_func)ism_write;
		io.ztell_file = (tell_file_func)ism_tell;
		io.zseek_file = (seek_file_func)ism_seek;
		io.zclose_file = (close_file_func)ism_close;
		io.zerror_file = (testerror_file_func)ism_testerror;
		io.alloc_mem = (alloc_func)ism_alloc;
		io.free_mem = (free_func)ism_free;

		unzFile pkg{ unzOpen2(m_packages[file.package].path.c_str(), &io) };
		ASSERT(pkg);
		i32 unz_err{ unzGoToFilePos(pkg, &file.file_pos) };
		if (unz_err != UNZ_OK || unzOpenCurrentFile(pkg) != UNZ_OK) {
			unzClose(pkg);
			ASSERT(nullptr);
		}

		return pkg;
	}

	Error_ ZipArchive::add_package(Path const & path)
	{
		return Error_OK;
	}

	bool ZipArchive::file_exists(Path const & path) const
	{
		return m_files.contains(path);
	}

	bool ZipArchive::try_open_pack(Path const & path, bool replace_files, u64 offset)
	{
		if (path.extension() != ".zip") { return false; }
		ASSERT((offset == 0) && "Invalid PCK data. Note that loading files with a non-zero offset isn't supported with ZIP archives.");
		
		zlib_filefunc_def io;
		memset(&io, 0, sizeof(io));
		io.opaque = nullptr;
		io.zopen_file = (open_file_func)ism_open;
		io.zread_file = (read_file_func)ism_read;
		io.zwrite_file = (write_file_func)ism_write;
		io.ztell_file = (tell_file_func)ism_tell;
		io.zseek_file = (seek_file_func)ism_seek;
		io.zclose_file = (close_file_func)ism_close;
		io.zerror_file = (testerror_file_func)ism_testerror;
		
		unzFile m_zfile = unzOpen2(path.c_str(), &io);
		ASSERT(m_zfile);
		
		unz_global_info64 gi;
		i32 err = unzGetGlobalInfo64(m_zfile, &gi);
		ASSERT(err == UNZ_OK);
		
		Package pkg;
		pkg.path = path;
		pkg.m_zfile = m_zfile;
		m_packages.push_back(pkg);
		i32 pkg_num = (i32)m_packages.size() - 1;
		
		for (u64 i = 0; i < gi.number_entry; ++i)
		{
			char filename_inzip[256];
		
			unz_file_info64 m_info;
			err = unzGetCurrentFileInfo64(m_zfile, &m_info, filename_inzip, sizeof(filename_inzip), nullptr, 0, nullptr, 0);
			ASSERT(err == UNZ_OK);
		
			File f;
			f.package = pkg_num;
			unzGetFilePos(m_zfile, &f.file_pos);
		
			Path fname{ filename_inzip };
			m_files[fname] = f;
		
			PACKED_DATA->add_path(path, fname, 1, 0, fname.hash_code(), this, replace_files, false);
			printf("packed data add path %s, %s\n", path.c_str(), fname.c_str());
		
			if ((i + 1) < gi.number_entry) {
				unzGoToNextFile(m_zfile);
			}
		}

		return false;
	}

	Ref<FileAccess> ZipArchive::get_file(Path const & path, PackedData::PackedFile * file)
	{
		return memnew(FileAccessZip(path, *file));
	}
}

namespace ism
{
	EMBED_CLASS(FileAccessZip, t) {}

	FileAccessZip::FileAccessZip(Path const & path, PackedData::PackedFile const & file)
	{
		open_internal(path, FileMode_Read);
	}

	FileAccessZip::~FileAccessZip()
	{
		close();
	}

	Error_ FileAccessZip::open_internal(Path const & path, FileMode_ mode)
	{
		m_zfile = VALIDATE(ZIP_ARCHIVE)->get_file_handle(path);
		ASSERT(m_zfile);
		ASSERT(UNZ_OK == unzGetCurrentFileInfo64(m_zfile, &m_info, nullptr, 0, nullptr, 0, nullptr, 0));
		return Error_OK;
	}

	void FileAccessZip::close()
	{
		if (!m_zfile) { return; }
		VALIDATE(ZIP_ARCHIVE)->close_handle(m_zfile);
		m_zfile = nullptr;
	}

	void FileAccessZip::flush()
	{
		CRASH("this should never be called");
	}

	bool FileAccessZip::exists(Path const & path)
	{
		return false;
	}

	bool FileAccessZip::is_open() const
	{
		return m_zfile;
	}

	void FileAccessZip::seek(u64 position)
	{
		ASSERT(m_zfile);
		unzSeekCurrentFile(m_zfile, (i32)position);
	}

	void FileAccessZip::seek_end(i64 position)
	{
		ASSERT(m_zfile);
		unzSeekCurrentFile(m_zfile, (i32)(get_length() + position));
	}

	u64 FileAccessZip::get_position() const
	{
		ASSERT(m_zfile);
		return unztell(m_zfile);
	}

	u64 FileAccessZip::get_length() const
	{
		ASSERT(m_zfile);
		return m_info.uncompressed_size;
	}

	bool FileAccessZip::eof_reached() const
	{
		ASSERT(m_zfile);
		return m_eof;
	}

	Error_ FileAccessZip::get_error() const
	{
		if (!m_zfile) {
			return Error_Unknown;
		}
		if (eof_reached()) {
			return Error_EOF;
		}
		return Error_OK;
	}

	u8 FileAccessZip::read_8() const
	{
		u8 temp{};
		read_buffer(&temp, 1);
		return temp;
	}

	u64 FileAccessZip::read_buffer(u8 * data, u64 size) const
	{
		ASSERT(data && size);
		ASSERT(m_zfile);

		m_eof = unzeof(m_zfile);
		if (m_eof) {
			return 0;
		}
		i64 const read{ unzReadCurrentFile(m_zfile, data, (i32)size) };
		ASSERT(-1 < read);
		if ((u64)read < size) {
			m_eof = true;
		}
		return read;
	}

	void FileAccessZip::write_8(u8 value)
	{
		CRASH("this should never be called");
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}