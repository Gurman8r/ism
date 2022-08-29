#include <core/io/file_access.hpp>

#include <filesystem>
#include <fstream>

#define BIG_ENDIAN false

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_OBJECT_CLASS(FileAccess, t, TypeFlags_IsAbstract) {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Error_ FileAccess::reopen(Path const & path, i32 flags)
	{
		return BRANCHLESS_IF(is_open(), close()), open(path, flags);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	u16 FileAccess::get_16() const
	{
		u8 a{ get_8() }, b{ get_8() };
		if (BIG_ENDIAN) { util::swap(a, b); }
		u16 c{ b }; c <<= 8; c |= a;
		return c;
	}

	u32 FileAccess::get_32() const
	{
		u16 a{ get_16() }, b{ get_16() };
		if (BIG_ENDIAN) { util::swap(a, b); }
		u32 c{ b }; c <<= 16; c |= a;
		return c;
	}

	u64 FileAccess::get_64() const
	{
		u32 a{ get_32() }, b{ get_32() };
		if (BIG_ENDIAN) { util::swap(a, b); }
		u64 c{ b }; c <<= 32; c |= a;
		return c;
	}

	f32 FileAccess::get_float() const
	{
		return f32();
	}

	f64 FileAccess::get_double() const
	{
		return f64();
	}

	String FileAccess::get_string() const
	{
		return String();
	}

	String FileAccess::get_line() const
	{
		return String();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void FileAccess::put_16(u16 value)
	{
		u8 a; a = value & 0xFF;
		u8 b; b = value >> 8;
		if (BIG_ENDIAN) { util::swap(a, b); }
		put_8(a); put_8(b);
	}

	void FileAccess::put_32(u32 value)
	{
		u16 a; a = value & 0xFFFF;
		u16 b; b = value >> 16;
		if (BIG_ENDIAN) { util::swap(a, b); }
		put_16(a); put_16(b);
	}

	void FileAccess::put_64(u64 value)
	{
		u32 a; a = value & 0xFFFFFFFF;
		u32 b; b = value >> 32;
		if (BIG_ENDIAN) { util::swap(a, b); }
		put_32(a); put_32(b);
	}

	void FileAccess::put_float(f32 value)
	{
	}

	void FileAccess::put_double(f64 value)
	{
	}

	void FileAccess::put_string(String const & value) const
	{
	}

	void FileAccess::put_line(String const & value) const
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_OBJECT_CLASS(FileDefault, t) {}

	FileDefault::FileDefault() {}

	FileDefault::FileDefault(Path const & path, i32 flags) { ASSERT(open(path, flags) == Error_None); }

	FileDefault::~FileDefault() { if (is_open()) { close(); } }

	bool FileDefault::is_open() const
	{
		return false;
	}

	Error_ FileDefault::open(Path const & path, i32 flags)
	{
		return Error_();
	}

	void FileDefault::close()
	{
	}

	u8 FileDefault::get_8() const
	{
		return u8();
	}

	void FileDefault::put_8(u8 value)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}