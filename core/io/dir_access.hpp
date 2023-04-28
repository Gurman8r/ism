#ifndef _ISM_DIR_ACCESS_HPP_
#define _ISM_DIR_ACCESS_HPP_

#include <core/object/class.hpp>

namespace ism
{
	// directory
	class ISM_API DirAccess : public Object
	{
		DEFINE_CLASS(DirAccess, Object);

	public:
		DirAccess();

		virtual ~DirAccess() override;
	};
}

#endif // !_ISM_DIR_ACCESS_HPP_