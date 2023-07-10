#ifndef _ISM_CAMERA_HPP_
#define _ISM_CAMERA_HPP_

#include <core/math/transform.hpp>

namespace Ism
{
	class Camera
	{
		Transform m_transform{};

	public:
		Camera() noexcept = default;
		Camera(Camera const &) = default;
		Camera(Camera &&) noexcept = default;
		Camera & operator=(Camera const &) = default;
		Camera & operator=(Camera &&) noexcept = default;
		Camera(Transform const & transform) : m_transform{ transform } {}
	};
}

#endif // !_ISM_CAMERA_HPP_
