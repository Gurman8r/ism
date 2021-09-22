#ifndef _ISM_CAMERA_SERVER_HPP_
#define _ISM_CAMERA_SERVER_HPP_

#include <core/detail/class.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API CameraServer : public Object
	{
		ISM_OBJECT(CameraServer, Object);

		static CameraServer * singleton;

	protected:
		explicit CameraServer() noexcept : Object{} { singleton = this; }

	public:
		virtual ~CameraServer() override { singleton = nullptr; }

		NODISCARD static CameraServer * get_singleton() noexcept { return singleton; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD CameraServer & get_camera_server() { return *CameraServer::get_singleton(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_CAMERA_SERVER_HPP_
