#ifndef _ISM_FILE_ACCESS_HPP_
#define _ISM_FILE_ACCESS_HPP_

#include <core/object/class.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum FileAccessType_ {
		FileAccessType_Resources,
		FileAccessType_UserData,
		FileAccessType_Filesystem,
		FileAccessType_MAX
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

	class ISM_API FileAccess : public Object
	{
		DEFINE_CLASS(FileAccess, Object);

		bool m_big_endian{};
		FileAccessType_ m_access_type{};

	protected:
		using CreateFunc = Ref<FileAccess>(*)();
		static CreateFunc __create_func[FileAccessType_MAX];
		static Ref<FileAccess> create(FileAccessType_ access_type);
		static Ref<FileAccess> create(Path const & path);
		virtual Error_ open_internal(Path const & path, FileMode_ mode) = 0;

		NODISCARD FileAccessType_ get_access_type() const noexcept { return m_access_type; }
		void set_access_type(FileAccessType_ value) noexcept { m_access_type = value; }

	public:
		static Ref<FileAccess> open(Path const & path, FileMode_ mode);
		virtual Error_ reopen(Path const & path, FileMode_ mode);

		virtual void flush() = 0;
		virtual void close() = 0;

		NODISCARD virtual bool exists(Path const & path) = 0;
		NODISCARD virtual bool is_open() const = 0;
		NODISCARD virtual Path get_path() const { return {}; }
		NODISCARD virtual Path get_path_absolute() const { return {}; }

		virtual void seek(u64 position) = 0;
		virtual void seek_end(i64 position = 0) = 0;

		NODISCARD virtual u64 get_position() const = 0;
		NODISCARD virtual u64 get_length() const = 0;
		NODISCARD virtual bool eof_reached() const = 0;
		NODISCARD virtual Error_ get_error() const = 0;

		NODISCARD bool is_big_endian() const noexcept { return m_big_endian; }
		virtual void set_big_endian(bool big_endian) noexcept { m_big_endian = big_endian; }

	public:
		NODISCARD virtual u8 read_8() const = 0;
		NODISCARD virtual u16 read_16() const;
		NODISCARD virtual u32 read_32() const;
		NODISCARD virtual u64 read_64() const;
		NODISCARD virtual f32 read_float() const;
		NODISCARD virtual f64 read_double() const;
		NODISCARD virtual String read_token() const;
		NODISCARD virtual String read_line() const;
		NODISCARD virtual size_t read_buffer(u8 * data, size_t const size) const;
		NODISCARD DynamicBuffer read_buffer(size_t const size) const;

		FileAccess & operator>>(bool & value) { return (value = (bool)read_8()), (*this); }
		FileAccess & operator>>(u8 & value) { return (value = read_8()), (*this); }
		FileAccess & operator>>(u16 & value) { return (value = read_16()), (*this); }
		FileAccess & operator>>(u32 & value) { return (value = read_32()), (*this); }
		FileAccess & operator>>(u64 & value) { return (value = read_64()), (*this); }
		FileAccess & operator>>(i8 & value) { return (value = (i8)read_8()), (*this); }
		FileAccess & operator>>(i16 & value) { return (value = (i16)read_16()), (*this); }
		FileAccess & operator>>(i32 & value) { return (value = (i32)read_32()), (*this); }
		FileAccess & operator>>(i64 & value) { return (value = (i64)read_64()), (*this); }
		FileAccess & operator>>(f32 & value) { return (value = read_float()), (*this); }
		FileAccess & operator>>(f64 & value) { return (value = read_double()), (*this); }
		FileAccess & operator>>(f80 & value) { return (value = (f80)read_double()), (*this); }
		FileAccess & operator>>(String & value) { return (value = read_token()), (*this); }

	public:
		virtual void write_8(u8 value) = 0;
		virtual void write_16(u16 value);
		virtual void write_32(u32 value);
		virtual void write_64(u64 value);
		virtual void write_float(f32 value);
		virtual void write_double(f64 value);
		virtual void write_token(String const & value);
		virtual void write_line(String const & value);
		virtual void write_buffer(u8 const * data, size_t const size);
		void write_buffer(DynamicBuffer const & buffer);

		FileAccess & operator<<(bool const value) { return write_8(value), (*this); }
		FileAccess & operator<<(u8 const value) { return  write_8(value), (*this); }
		FileAccess & operator<<(u16 const value) { return write_16(value), (*this); }
		FileAccess & operator<<(u32 const value) { return write_32(value), (*this); }
		FileAccess & operator<<(u64 const value) { return write_64(value), (*this); }
		FileAccess & operator<<(i8 const value) { return  write_8((u8)value), (*this); }
		FileAccess & operator<<(i16 const value) { return write_16((u16)value), (*this); }
		FileAccess & operator<<(i32 const value) { return write_32((u32)value), (*this); }
		FileAccess & operator<<(i64 const value) { return write_64((u64)value), (*this); }
		FileAccess & operator<<(f32 const value) { return write_float(value), (*this); }
		FileAccess & operator<<(f64 const value) { return write_double(value), (*this); }
		FileAccess & operator<<(f80 const value) { return write_double((f64)value), (*this); }
		FileAccess & operator<<(String const & value) { return write_token(value), (*this); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_FILE_ACCESS_HPP_
