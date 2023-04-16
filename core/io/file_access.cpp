#include <core/io/file_access.hpp>

#include <cstdio>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(FileAccess, t, TypeFlags_IsAbstract) {}

	decltype(FileAccess::__create_func) FileAccess::__create_func{};

	Ref<FileAccess> FileAccess::create(FileAccessType_ access_type)
	{
		Ref<FileAccess> file{ VALIDATE(__create_func[access_type])() };
		file->set_access_type(access_type);
		return file;
	}

	Ref<FileAccess> FileAccess::create(Path const & path)
	{
		Ref<FileAccess> file;
		file = create(FileAccessType_Filesystem);
		return file;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Ref<FileAccess> FileAccess::open(Path const & path, FileMode_ mode)
	{
		Ref<FileAccess> file{ create(path) };
		if (file->open_internal(path, mode) != Error_None) {
			file = nullptr;
		}
		return file;
	}

	Error_ FileAccess::reopen(Path const & path, FileMode_ mode)
	{
		close();
		open(path, mode);
		return Error_None;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	u16 FileAccess::read_16() const
	{
		u8 a{ read_8() }, b{ read_8() };
		if (m_big_endian) { util::swap(a, b); }
		u16 c{ b }; c <<= 8; c |= a;
		return c;
	}

	u32 FileAccess::read_32() const
	{
		u16 a{ read_16() }, b{ read_16() };
		if (m_big_endian) { util::swap(a, b); }
		u32 c{ b }; c <<= 16; c |= a;
		return c;
	}

	u64 FileAccess::read_64() const
	{
		u32 a{ read_32() }, b{ read_32() };
		if (m_big_endian) { util::swap(a, b); }
		u64 c{ b }; c <<= 32; c |= a;
		return c;
	}

	f32 FileAccess::read_float() const
	{
		return f32();
	}

	f64 FileAccess::read_double() const
	{
		return f64();
	}

	String FileAccess::read_string() const
	{
		return String();
	}

	String FileAccess::read_line() const
	{
		return String();
	}

	String FileAccess::read_token() const
	{
		return String();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void FileAccess::write_16(u16 value)
	{
		u8 a; a = value & 0xFF;
		u8 b; b = value >> 8;
		if (m_big_endian) { util::swap(a, b); }
		write_8(a); write_8(b);
	}

	void FileAccess::write_32(u32 value)
	{
		u16 a; a = value & 0xFFFF;
		u16 b; b = value >> 16;
		if (m_big_endian) { util::swap(a, b); }
		write_16(a); write_16(b);
	}

	void FileAccess::write_64(u64 value)
	{
		u32 a; a = value & 0xFFFFFFFF;
		u32 b; b = value >> 32;
		if (m_big_endian) { util::swap(a, b); }
		write_32(a); write_32(b);
	}

	void FileAccess::write_float(f32 value)
	{
	}

	void FileAccess::write_double(f64 value)
	{
	}

	void FileAccess::write_string(String const & value) const
	{
	}

	void FileAccess::write_line(String const & value) const
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}