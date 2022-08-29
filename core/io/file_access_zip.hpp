#ifndef _ISM_FILE_ACCESS_ZIP_HPP_
#define _ISM_FILE_ACCESS_ZIP_HPP_

// PLACEHOLDER

#include <core/io/file_access.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API FileAccessZip : public FileAccess
	{
		OBJECT_CLASS(FileAccessZip, FileAccess);

	public:
		FileAccessZip();
		virtual ~FileAccessZip() override;

		virtual bool is_open() const override;
		virtual Error_ open(Path const & path, i32 flags) override;
		virtual void close() override;
		virtual u8	get_8() const override;
		virtual void put_8(u8 value) override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_FILE_ACCESS_ZIP_HPP_
