#ifndef _ISM_DIR_HPP_
#define _ISM_DIR_HPP_

#include <core/object/detail/class.hpp>

namespace ism
{
	// directory
	class ISM_API Dir : public Object
	{
		OBJECT_CLASS(Dir, Object);

	public:
		Dir();

		virtual ~Dir() override;
	};
}

#endif // !_ISM_DIR_HPP_
