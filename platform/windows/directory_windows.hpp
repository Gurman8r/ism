#ifndef _ISM_DIRECTORY_WINDOWS_HPP_
#define _ISM_DIRECTORY_WINDOWS_HPP_

#include <core/os/directory.hpp>

namespace ism
{
	// directory windows
	class ISM_API DirectoryWindows : public Directory
	{
		OBJECT_CLASS(DirectoryWindows, Directory);

	protected:
		DirectoryWindows();

	public:
		virtual ~DirectoryWindows() override;


	};
}

#endif // !_ISM_DIRECTORY_WINDOWS_HPP_
