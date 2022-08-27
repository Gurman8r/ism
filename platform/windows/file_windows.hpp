#ifndef _ISM_FILE_WINDOWS_HPP_
#define _ISM_FILE_WINDOWS_HPP_

#include <core/os/file.hpp>

namespace ism
{
	// file windows
	class ISM_API FileWindows : public File
	{
		OBJECT_CLASS(FileWindows, File);

	protected:
		FileWindows();

	public:
		virtual ~FileWindows() override;
	};
}

#endif // !_ISM_FILE_WINDOWS_HPP_
