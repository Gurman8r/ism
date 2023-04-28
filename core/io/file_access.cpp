#include <core/io/file_access.hpp>

#include <cstdio>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(FileAccess, t, TypeFlags_IsAbstract) {}

	FileAccess::CreateFunc FileAccess::__create_func[FileAccessType_MAX]{};

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
		if (Ref<FileAccess> file{ create(path) }
		; file->open_internal(path, mode) == Error_OK) {
			return file;
		}
		return nullptr;
	}

	Error_ FileAccess::reopen(Path const & path, FileMode_ mode)
	{
		return open_internal(path, mode);
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

	String FileAccess::read_token() const
	{
		String token{};
		u8 c{ read_8() };
		while (!eof_reached()) {
			if (c == ' ' && !token.empty()) { break; }
			else { token.push_back(c); }
			c = read_8();
		}
		return token;
	}

	String FileAccess::read_line() const
	{
		String line{};
		u8 c{ read_8() };
		while (!eof_reached()) {
			if (c == '\n' || c == '\0') { return line.push_back(0), line; }
			else if (c != '\r') { line.push_back(c); }
			c = read_8();
		}
		if (!line.empty()) { line.push_back(0); }
		return line;
	}

	size_t FileAccess::read_buffer(u8 * data, size_t const size) const
	{
		size_t i{};
		for (; i < size && !eof_reached(); ++i) {
			data[i] = read_8();
		}
		return i;
	}

	DynamicBuffer FileAccess::read_buffer(size_t const size) const
	{
		DynamicBuffer buf{};
		if (!size) { return buf; }
		buf.resize(size);
		if (size_t length{ read_buffer(buf.data(), buf.size()) }; length < size) {
			buf.resize(length);
		}
		return buf;
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

	void FileAccess::write_token(String const & value)
	{
		if (value.empty()) { return; }
		write_buffer((u8 *)value.data(), value.size());
	}

	void FileAccess::write_line(String const & value)
	{
		write_token(value);
		write_8((u8)'\n');
	}

	void FileAccess::write_buffer(u8 const * data, size_t const size)
	{
		if (!data || !size) { return; }
		for (size_t i{}; i < size; ++i) { write_8(data[i]); }
	}

	void FileAccess::write_buffer(DynamicBuffer const & buffer)
	{
		if (buffer.empty()) { return; }
		write_buffer(buffer.data(), buffer.size());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}