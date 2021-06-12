#ifndef _ISM_CAMERA_SERVER_HPP_
#define _ISM_CAMERA_SERVER_HPP_

#include <core/api/super.hpp>

namespace ism
{
	class ISM_API CameraServer : public Super
	{
		ISM_SUPER_CLASS(CameraServer, Super);

	public:
		virtual ~CameraServer() override;

		DEFAULT_COPY_AND_MOVE_CONSTRUCTABLE(CameraServer);
	};
}

#endif // !_ISM_CAMERA_SERVER_HPP_
