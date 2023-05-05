#include <core/io/file.hpp>
#include <core/io/package.hpp>
#include <cstdio>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(File, t, TypeFlags_IsAbstract) {}

	File::CreateFunc File::__create_func[FileType_MAX]{};

	Ref<File> File::create(FileType_ access_type)
	{
		Ref<File> file{ VALIDATE(__create_func[access_type])() };
		file->set_access_type(access_type);
		return file;
	}

	Ref<File> File::create_for_path(String const & path)
	{
		Ref<File> file;
		if (path.has_prefix("res://")) {
			file = create(FileType_Resources);
		}
		else if (path.has_prefix("usr://")) {
			file = create(FileType_User);
		}
		else {
			file = create(FileType_Filesystem);
		}
		return file;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Ref<File> File::open(String const & path, FileMode_ mode, Error_ * error)
	{
		Ref<File> file{};
		
		if (PackageManager * pack; (mode != FileMode_Write) && (pack = get_packages()) && pack->is_enabled() && (file = pack->try_open_path(path))) {
			if (error) { *error = Error_OK; }
			return file;
		}

		if ((file = create_for_path(path)) && (file->open_internal(path, mode) == Error_OK)) {
			if (error) { *error = Error_OK; }
			return file;
		}

		if (error) { *error = Error_Unknown; }
		return nullptr;
	}

	Error_ File::reopen(String const & path, FileMode_ mode)
	{
		return open_internal(path, mode);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	u16 File::read_16() const
	{
		u8 a{ read_8() }, b{ read_8() };
		if (m_big_endian) { util::swap(a, b); }
		u16 c{ b }; c <<= 8; c |= a;
		return c;
	}

	u32 File::read_32() const
	{
		u16 a{ read_16() }, b{ read_16() };
		if (m_big_endian) { util::swap(a, b); }
		u32 c{ b }; c <<= 16; c |= a;
		return c;
	}

	u64 File::read_64() const
	{
		u32 a{ read_32() }, b{ read_32() };
		if (m_big_endian) { util::swap(a, b); }
		u64 c{ b }; c <<= 32; c |= a;
		return c;
	}

	f32 File::read_float() const
	{
		return f32();
	}

	f64 File::read_double() const
	{
		return f64();
	}

	String File::read_token() const
	{
		String token{};
		u8 c{ read_8() };
		while (!eof_reached()) {
			if (c == ' ' && !token.empty()) { break; }
			else { token += c; }
			c = read_8();
		}
		return token;
	}

	String File::read_line() const
	{
		String line{};
		u8 c{ read_8() };
		while (!eof_reached()) {
			if (c == '\0' || c == '\n') {
				line += '\n';
				return line;
			}
			else if (c != '\r') {
				line += c;
			}
			c = read_8();
		}
		return line;
	}

	size_t File::read_buffer(u8 * data, size_t const size) const
	{
		size_t i{};
		for (; i < size && !eof_reached(); ++i) {
			data[i] = read_8();
		}
		return i;
	}

	DynamicBuffer File::read_buffer(size_t const size) const
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

	File & File::write_16(u16 value)
	{
		u8 a; a = value & 0xFF;
		u8 b; b = value >> 8;
		if (m_big_endian) { util::swap(a, b); }
		return write_8(a); write_8(b);
	}

	File & File::write_32(u32 value)
	{
		u16 a; a = value & 0xFFFF;
		u16 b; b = value >> 16;
		if (m_big_endian) { util::swap(a, b); }
		return write_16(a).write_16(b);
	}

	File & File::write_64(u64 value)
	{
		u32 a; a = value & 0xFFFFFFFF;
		u32 b; b = value >> 32;
		if (m_big_endian) { util::swap(a, b); }
		return write_32(a).write_32(b);
	}

	File & File::write_float(f32 value)
	{
		return (*this);
	}

	File & File::write_double(f64 value)
	{
		return (*this);
	}

	File & File::write_token(String const & value)
	{
		if (value.empty()) { return (*this); }
		else { return write_buffer((u8 *)value.data(), value.size()); }
	}

	File & File::write_line(String const & value)
	{
		return write_token(value).write_8((u8)'\n');
	}

	File & File::write_buffer(u8 const * data, size_t const size)
	{
		if (!data || !size) { return (*this); }
		for (size_t i{}; i < size; ++i) { write_8(data[i]); }
		return (*this);
	}

	File & File::write_buffer(DynamicBuffer const & buffer)
	{
		if (buffer.empty()) { return (*this); }
		else { return write_buffer(buffer.data(), buffer.size()); }
	}

	bool File::exists(String const & path)
	{
		return (get_packages() && get_packages()->has_path(path))
			|| open(path, FileMode_Read).is_valid();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}