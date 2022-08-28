#ifndef _ISM_FILE_HPP_
#define _ISM_FILE_HPP_

#include <core/object/detail/class.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// file
	class ISM_API File : public Object
	{
		OBJECT_CLASS(File, Object);

	public:
		enum Access_
		{
			Access_Read			= 1 << 0,
			Access_Write		= 1 << 1,
			Access_ReadWrite	= 1 << 2,
			Access_WriteRead	= 1 << 3,
		};

	public:
		File() noexcept {}
		virtual ~File() noexcept override = default;

	public:
		virtual Path get_path() const noexcept { return ""_path; }
		virtual Path get_path_absolute() const noexcept { return ""_path; }

	public:
		virtual bool is_open() const = 0;
		virtual Error_ open(Path const & path, i32 flags) = 0;
		virtual Error_ reopen(Path const & path, i32 flags);
		virtual void close() = 0;

	public:
		virtual u8	get_8() const = 0;
		virtual u16	get_16() const;
		virtual u32	get_32() const;
		virtual u64	get_64() const;
		virtual f32 get_float() const;
		virtual f64 get_double() const;
		virtual String get_string() const;
		virtual String get_line() const;

	public:
		virtual void put_8(u8 value) = 0;
		virtual void put_16(u16 value);
		virtual void put_32(u32 value);
		virtual void put_64(u64 value);
		virtual void put_float(f32 value);
		virtual void put_double(f64 value);
		virtual void put_string(String const & value) const;
		virtual void put_line(String const & value) const;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// file default
	class ISM_API FileDefault : public File
	{
		OBJECT_CLASS(FileDefault, File);

	public:
		FileDefault();
		FileDefault(Path const & path, i32 flags);
		virtual ~FileDefault() override;
		virtual bool is_open() const override;
		virtual Error_ open(Path const & path, i32 flags) override;
		virtual void close() override;
		virtual u8	get_8() const override;
		virtual void put_8(u8 value) override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_FILE_HPP_
