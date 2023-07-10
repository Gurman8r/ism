#include <core/io/file.hpp>
#include <core/io/pack.hpp>
#include <core/os/os.hpp>
#include <core/config/project_settings.hpp>
#include <cstdio>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(File, t, TypeFlags_IsAbstract) {}

	File::CreateFunc File::__create_func[FileAccess_MAX]{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	String File::fix_path(String const & path) const
	{
		String const r_path{ path.replace('\\', '/') };
		switch (m_access_type)
		{
		case FileAccess_Resources: {
			if (globals()) {
				if (r_path.begins_with("res://")) {
					String resource_path{ os()->get_resource_path() };
					if (!resource_path.empty()) {
						return r_path.replace("res:/", resource_path);
					}
					return r_path.replace("res://", "");
				}
			}

		} break;
		case FileAccess_User: {
			if (r_path.begins_with("user://")) {
				if (String data_dir{ os()->get_user_path() }; !data_dir.empty()) {
					return r_path.replace("user:/", data_dir);
				}
				return r_path.replace("user://", "");
			}
		} break;
		case FileAccess_Filesystem: {
			return r_path;
		} break;
		case FileAccess_MAX: {
			/* can't happen, but silences warning */
		} break;
		}
		return r_path;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Ref<File> File::create(FileAccess_ access_type)
	{
		Ref<File> file{ VALIDATE(__create_func[access_type])() };
		file->set_access_type(access_type);
		return file;
	}

	Ref<File> File::create_for_path(String const & path)
	{
		Ref<File> file;
		if (path.begins_with("res://")) {
			file = create(FileAccess_Resources);
		}
		else if (path.begins_with("user://")) {
			file = create(FileAccess_User);
		}
		else {
			file = create(FileAccess_Filesystem);
		}
		return file;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Ref<File> File::open(String const & path, FileMode_ mode, Error_ * r_error)
	{
		Ref<File> file{};
		
		if (PackedData * pack; (mode != FileMode_Write) && (pack = PackedData::get_singleton()) && pack->is_enabled() && (file = pack->try_open_path(path))) {
			if (r_error) { *r_error = Error_OK; }
			return file;
		}

		if ((file = create_for_path(path)) && (file->open_internal(path, mode) == Error_OK)) {
			if (r_error) { *r_error = Error_OK; }
			return file;
		}

		if (r_error) { *r_error = Error_Failed; }
		return nullptr;
	}

	Error_ File::reopen(String const & path, FileMode_ mode)
	{
		return open_internal(path, mode);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	u16 File::get_16() const
	{
		u8 a{ get_8() }, b{ get_8() };
		if (m_big_endian) { util::swap(a, b); }
		u16 c{ b }; c <<= 8; c |= a;
		return c;
	}

	u32 File::get_32() const
	{
		u16 a{ get_16() }, b{ get_16() };
		if (m_big_endian) { util::swap(a, b); }
		u32 c{ b }; c <<= 16; c |= a;
		return c;
	}

	u64 File::get_64() const
	{
		u32 a{ get_32() }, b{ get_32() };
		if (m_big_endian) { util::swap(a, b); }
		u64 c{ b }; c <<= 32; c |= a;
		return c;
	}

	f32 File::get_float() const
	{
		return f32();
	}

	f64 File::get_double() const
	{
		return f64();
	}

	String File::get_token() const
	{
		String token{};
		u8 c{ get_8() };
		while (!eof_reached()) {
			if (c == ' ' && !token.empty()) { break; }
			else { token += c; }
			c = get_8();
		}
		return token;
	}

	String File::get_line() const
	{
		String line{};
		u8 c{ get_8() };
		while (!eof_reached()) {
			if (c == '\0' || c == '\n') {
				line += '\n';
				return line;
			}
			else if (c != '\r') {
				line += c;
			}
			c = get_8();
		}
		return line;
	}

	size_t File::get_buffer(u8 * data, size_t const size) const
	{
		size_t i{};
		for (; i < size && !eof_reached(); ++i) {
			data[i] = get_8();
		}
		return i;
	}

	DynamicBuffer File::get_buffer(size_t const size) const
	{
		DynamicBuffer buf{};
		if (!size) { return buf; }
		buf.resize(size);
		if (size_t length{ get_buffer(buf.data(), buf.size()) }; length < size) {
			buf.resize(length);
		}
		return buf;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	File & File::put_16(u16 value)
	{
		u8 a; a = value & 0xFF;
		u8 b; b = value >> 8;
		if (m_big_endian) { util::swap(a, b); }
		return put_8(a).put_8(b);
	}

	File & File::put_32(u32 value)
	{
		u16 a; a = value & 0xFFFF;
		u16 b; b = value >> 16;
		if (m_big_endian) { util::swap(a, b); }
		return put_16(a).put_16(b);
	}

	File & File::put_64(u64 value)
	{
		u32 a; a = value & 0xFFFFFFFF;
		u32 b; b = value >> 32;
		if (m_big_endian) { util::swap(a, b); }
		return put_32(a).put_32(b);
	}

	File & File::put_float(f32 value)
	{
		return (*this);
	}

	File & File::put_double(f64 value)
	{
		return (*this);
	}

	File & File::put_token(String const & value)
	{
		if (value.empty()) { return (*this); }
		else { return put_buffer((u8 *)value.data(), value.size()); }
	}

	File & File::put_line(String const & value)
	{
		return put_token(value).put_8((u8)'\n');
	}

	File & File::put_buffer(u8 const * data, size_t const size)
	{
		if (!data || !size) { return (*this); }
		for (size_t i{}; i < size; ++i) { put_8(data[i]); }
		return (*this);
	}

	File & File::put_buffer(DynamicBuffer const & buffer)
	{
		if (buffer.empty()) { return (*this); }
		else { return put_buffer(buffer.data(), buffer.size()); }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	File::CreateFunc File::get_create_func(FileAccess_ access)
	{
		return CreateFunc();
	}

	bool File::exists(String const & path)
	{
		return (PackedData::get_singleton() && PackedData::get_singleton()->has_path(path)) || open(path, FileMode_Read).is_valid();
	}

	u64 File::get_modified_time(String const & path)
	{
		if (PackedData::get_singleton() && PackedData::get_singleton()->is_enabled() && (PackedData::get_singleton()->has_path(path) || PackedData::get_singleton()->has_dir(path))) {
			return 0;
		}

		Ref<File> fa{ create_for_path(path) };
		if (fa.is_null()) {
			CRASH("cannot create file for path");
		}

		u64 mt{ fa->_get_modified_time(path) };
		return mt;
	}

	u32 File::get_unix_permissions(String const & path)
	{
		if (PackedData::get_singleton() && PackedData::get_singleton()->is_enabled() && (PackedData::get_singleton()->has_path(path) || PackedData::get_singleton()->has_dir(path))) {
			return 0;
		}

		Ref<File> fa{ create_for_path(path) };
		if (fa.is_null()) {
			CRASH("cannot create file for path");
		}

		uint32_t mt = fa->_get_unix_permissions(path);
		return mt;
	}

	Error_ File::set_unix_permissions(String const & path, u32 permissions)
	{
		if (PackedData::get_singleton() && PackedData::get_singleton()->is_enabled() && (PackedData::get_singleton()->has_path(path) || PackedData::get_singleton()->has_dir(path))) {
			return Error_Unavailable;
		}

		Ref<File> fa{ create_for_path(path) };
		if (fa.is_null()) {
			CRASH("cannot create file for path");
		}

		return fa->_set_unix_permissions(path, permissions);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}