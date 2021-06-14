#ifndef _ISM_CAMERA_SERVER_HPP_
#define _ISM_CAMERA_SERVER_HPP_

#include <core/api/modsupport.hpp>

namespace ism
{
	class ISM_API CameraServer : public Super
	{
		ISM_SUPER(CameraServer, Super);

	public:
		virtual ~CameraServer() override;

		DEFAULT_COPYABLE_MOVABLE(CameraServer);
	};
}

#endif // !_ISM_CAMERA_SERVER_HPP_
