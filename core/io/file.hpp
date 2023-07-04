#ifndef _ISM_FILE_HPP_
#define _ISM_FILE_HPP_

#include <core/object/class.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum FileAccess_ {
		FileAccess_Resources,
		FileAccess_User,
		FileAccess_Filesystem,
		FileAccess_MAX
	};

	enum FileMode_ {
		FileMode_Read,		// r
		FileMode_Write,		// w
		FileMode_ReadWrite,	// r+
		FileMode_WriteRead,	// w+
		FileMode_MAX
	};

	enum FileCompression_ {
		FileCompression_FastLZ,
		FileCompression_Deflate,
		FileCompression_ZSTD,
		FileCompression_GZip,
		FileCompression_MAX
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// file access
	class ISM_API File : public Object
	{
		DEFINE_CLASS(File, Object);

		bool m_big_endian{};
		FileAccess_ m_access_type{};

	protected:
		NODISCARD virtual String fix_path(String const & path) const;
		virtual Error_ open_internal(String const & path, FileMode_ mode) = 0;
		NODISCARD virtual u64 _get_modified_time(String const & path) = 0;
		NODISCARD virtual u32 _get_unix_permissions(String const & path) = 0;
		virtual Error_ _set_unix_permissions(String const & path, u32 permissions) = 0;
		NODISCARD FileAccess_ get_access_type() const noexcept { return m_access_type; }
		void set_access_type(FileAccess_ value) noexcept { m_access_type = value; }

	protected:
		using CreateFunc = Ref<File>(*)();
		static CreateFunc __create_func[FileAccess_MAX];
		static Ref<File> create(FileAccess_ access_type);
		static Ref<File> create_for_path(String const & path);

	public:
		static Ref<File> open(String const & path, FileMode_ mode, Error_ * r_error = nullptr);
		virtual Error_ reopen(String const & path, FileMode_ mode);

		virtual File & flush() = 0;
		virtual File & close() = 0;
		NODISCARD virtual bool file_exists(String const & path) = 0;
		NODISCARD virtual bool is_open() const = 0;
		virtual File & seek(u64 position) = 0;
		virtual File & seek_end(i64 position = 0) = 0;
		NODISCARD virtual u64 get_position() const = 0;
		NODISCARD virtual u64 get_length() const = 0;
		NODISCARD virtual bool eof_reached() const = 0;
		NODISCARD virtual Error_ get_error() const = 0;

		NODISCARD virtual String get_path() const { return {}; }
		NODISCARD virtual String get_path_abs() const { return {}; }

		NODISCARD bool is_big_endian() const noexcept { return m_big_endian; }
		virtual void set_big_endian(bool big_endian) noexcept { m_big_endian = big_endian; }

		NODISCARD virtual u8 get_8() const = 0;
		NODISCARD virtual u16 get_16() const;
		NODISCARD virtual u32 get_32() const;
		NODISCARD virtual u64 get_64() const;
		NODISCARD virtual f32 get_float() const;
		NODISCARD virtual f64 get_double() const;
		NODISCARD virtual String get_token() const;
		NODISCARD virtual String get_line() const;
		virtual size_t get_buffer(u8 * data, size_t const size) const;
		NODISCARD DynamicBuffer get_buffer(size_t const size) const;

		virtual File & put_8(u8 value) = 0;
		virtual File & put_16(u16 value);
		virtual File & put_32(u32 value);
		virtual File & put_64(u64 value);
		virtual File & put_float(f32 value);
		virtual File & put_double(f64 value);
		virtual File & put_token(String const & value);
		virtual File & put_line(String const & value);
		virtual File & put_buffer(u8 const * data, size_t const size);
		File & put_buffer(DynamicBuffer const & buffer);

	public:
		static CreateFunc get_create_func(FileAccess_ p_access);
		static bool exists(String const & path);
		static u64 get_modified_time(String const & path);
		static u32 get_unix_permissions(String const & path);
		static Error_ set_unix_permissions(String const & path, u32 permissions);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_FILE_HPP_
