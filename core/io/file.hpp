#ifndef _ISM_FILE_HPP_
#define _ISM_FILE_HPP_

#include <core/object/class.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum FileType_ {
		FileType_Resources,
		FileType_User,
		FileType_Filesystem,
		FileType_MAX
	};

	enum FileMode_ {
		FileMode_Read,		// r
		FileMode_Write,		// w
		FileMode_ReadWrite,	// r+
		FileMode_WriteRead,	// w+
		FileMode_MAX
	};

	enum FileCompressionMode_ {
		FileCompressionMode_FastLZ,
		FileCompressionMode_Deflate,
		FileCompressionMode_ZSTD,
		FileCompressionMode_GZip,
		FileCompressionMode_MAX
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// file access
	class ISM_API File : public Object
	{
		DEFINE_CLASS(File, Object);

		bool m_big_endian{};
		FileType_ m_access_type{};

	protected:
		using CreateFunc = Ref<File>(*)();
		static CreateFunc __create_func[FileType_MAX];
		static Ref<File> create(FileType_ access_type);
		static Ref<File> create_for_path(String const & path);

		NODISCARD FileType_ get_access_type() const noexcept { return m_access_type; }
		void set_access_type(FileType_ value) noexcept { m_access_type = value; }

		virtual Error_ open_internal(String const & path, FileMode_ mode) = 0;

	public:
		static Ref<File> open(String const & path, FileMode_ mode, Error_ * error = nullptr);
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

		NODISCARD virtual u8 read_8() const = 0;
		NODISCARD virtual u16 read_16() const;
		NODISCARD virtual u32 read_32() const;
		NODISCARD virtual u64 read_64() const;
		NODISCARD virtual f32 read_float() const;
		NODISCARD virtual f64 read_double() const;
		NODISCARD virtual String read_token() const;
		NODISCARD virtual String read_line() const;
		virtual size_t read_buffer(u8 * data, size_t const size) const;
		NODISCARD DynamicBuffer read_buffer(size_t const size) const;

		virtual File & write_8(u8 value) = 0;
		virtual File & write_16(u16 value);
		virtual File & write_32(u32 value);
		virtual File & write_64(u64 value);
		virtual File & write_float(f32 value);
		virtual File & write_double(f64 value);
		virtual File & write_token(String const & value);
		virtual File & write_line(String const & value);
		virtual File & write_buffer(u8 const * data, size_t const size);
		File & write_buffer(DynamicBuffer const & buffer);

	public:
		static bool exists(String const & path);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_FILE_HPP_
