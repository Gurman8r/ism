#ifndef _ISM_DISPLAY_SERVER_HPP_
#define _ISM_DISPLAY_SERVER_HPP_

#include <core/api/object/generic_object.hpp>

namespace ism
{
	class ISM_API DisplayServer// : public Super
	{
		//ISM_SUPER(DisplayServer, Super);

	public:
		virtual ~DisplayServer();

		DEFAULT_COPYABLE_MOVABLE(DisplayServer);
	};
}

#endif // !_ISM_DISPLAY_SERVER_HPP_
