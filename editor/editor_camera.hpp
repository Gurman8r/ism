#ifndef _ISM_EDITOR_CAMERA_HPP_
#define _ISM_EDITOR_CAMERA_HPP_

#include <servers/camera_server.hpp>

namespace ism
{
	class ISM_API EditorCamera : public CameraController
	{
		OBJECT_COMMON(EditorCamera, CameraController);

	public:
		EditorCamera(Camera * ptr = {}) : CameraController{ ptr } {}

		virtual ~EditorCamera();
	};
}

#endif // !_ISM_EDITOR_CAMERA_HPP_
