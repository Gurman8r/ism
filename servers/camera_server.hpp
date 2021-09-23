#ifndef _ISM_CAMERA_SERVER_HPP_
#define _ISM_CAMERA_SERVER_HPP_

#include <core/object/detail/class.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API CameraServer : public Object
	{
		OBJ_COMMON(CameraServer, Object);

		static CameraServer * singleton;

	protected:
		explicit CameraServer() noexcept : Object{} { singleton = this; }

	public:
		virtual ~CameraServer() override {}

		NODISCARD static CameraServer * get_singleton() noexcept { return singleton; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD inline CameraServer & get_camera_server() { return *CHECK(CameraServer::get_singleton()); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_CAMERA_SERVER_HPP_
