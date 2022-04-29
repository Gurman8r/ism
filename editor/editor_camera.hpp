#ifndef _ISM_EDITOR_CAMERA_HPP_
#define _ISM_EDITOR_CAMERA_HPP_

#include <servers/camera_server.hpp>

namespace ism
{
	class ISM_API EditorCamera : public Camera
	{
		OBJECT_COMMON(EditorCamera, Camera);

	public:
		virtual ~EditorCamera();
	};
}

#endif // !_ISM_EDITOR_CAMERA_HPP_
