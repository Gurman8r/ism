#include <core/io/file_access_zip.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_OBJECT_CLASS(FileAccessZip, t) {}

	FileAccessZip::FileAccessZip()
	{
	}

	FileAccessZip::~FileAccessZip()
	{
	}

	bool FileAccessZip::is_open() const
	{
		return false;
	}

	Error_ FileAccessZip::open(Path const & path, i32 flags)
	{
		return Error_();
	}

	void FileAccessZip::close()
	{
	}

	u8 FileAccessZip::get_8() const
	{
		return u8();
	}

	void FileAccessZip::put_8(u8 value)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}