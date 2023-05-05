#ifndef _ISM_DIRECTORY_HPP_
#define _ISM_DIRECTORY_HPP_

#include <core/object/class.hpp>

namespace Ism
{
	// dir access
	class ISM_API Directory : public Object
	{
		DEFINE_CLASS(Directory, Object);

	protected:
		using CreateFunc = Ref<Directory>(*)();
		static CreateFunc __create_func;
		static Ref<Directory> create();
		static Ref<Directory> create_for_path(String const & path);

	public:
		static Ref<Directory> open(String const & path, Error_ * error = nullptr);
	};
}

#endif // !_ISM_DIRECTORY_HPP_
