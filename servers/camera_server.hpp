#ifndef _ISM_CAMERA_SERVER_HPP_
#define _ISM_CAMERA_SERVER_HPP_

#include <core/api/class.hpp>

namespace ism
{
	class ISM_API CameraServer : public Super
	{
		ISM_SUPER(CameraServer, Super);

		static CameraServer * singleton;

	public:
		virtual ~CameraServer();

		static CameraServer * get_singleton() { return singleton; }
	};
}

#endif // !_ISM_CAMERA_SERVER_HPP_
