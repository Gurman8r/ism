#ifndef _ISM_CAMERA_SERVER_HPP_
#define _ISM_CAMERA_SERVER_HPP_

#include <core/api/object/generic_object.hpp>

namespace ism
{
	class ISM_API CameraServer// : public Super
	{
		//ISM_SUPER(CameraServer, Super);

	public:
		virtual ~CameraServer();

		DEFAULT_COPYABLE_MOVABLE(CameraServer);
	};
}

#endif // !_ISM_CAMERA_SERVER_HPP_
