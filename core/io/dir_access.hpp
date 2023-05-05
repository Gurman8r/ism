#ifndef _ISM_DIR_ACCESS_HPP_
#define _ISM_DIR_ACCESS_HPP_

#include <core/object/class.hpp>

namespace Ism
{
	// dir access
	class ISM_API DirAccess : public Object
	{
		DEFINE_CLASS(DirAccess, Object);

	protected:
		using CreateFunc = Ref<DirAccess>(*)();
		static CreateFunc __create_func;
		static Ref<DirAccess> create();
		static Ref<DirAccess> create_for_path(String const & path);

	public:
		static Ref<DirAccess> open(String const & path, Error_ * error = nullptr);
	};
}

#endif // !_ISM_DIR_ACCESS_HPP_
