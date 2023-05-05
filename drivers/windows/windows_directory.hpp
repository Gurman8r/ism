#ifndef _ISM_WINDOWS_DIRECTORY_HPP_
#define _ISM_WINDOWS_DIRECTORY_HPP_

#include <core/io/directory.hpp>

namespace Ism
{
	// directory
	class ISM_API WindowsDir : public Directory
	{
		DEFINE_CLASS(WindowsDir, Directory);

	public:
		WindowsDir();
		virtual ~WindowsDir() override;

		static void initialize();
		static void finalize();
	};
}

#endif // !_ISM_WINDOWS_DIRECTORY_HPP_
