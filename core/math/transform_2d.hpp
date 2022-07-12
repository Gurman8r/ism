#ifndef _ISM_TRANSFORM_2D_HPP_
#define _ISM_TRANSFORM_2D_HPP_

#include <core/math/transform.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Transform2D : public Transform
	{
	public:
		explicit Transform2D(
			f32 a00, f32 a01, f32 a02,
			f32 a10, f32 a11, f32 a12,
			f32 a20, f32 a21, f32 a22
		) noexcept : Transform{
			a00, a01, a02,
			a10, a11, a12,
			a20, a21, a22
		} {}

		Transform2D() noexcept : Transform{} {}

		Transform2D(Mat4 const & value) : Transform{ value } {}

		Transform2D(Transform const & value) : Transform{ value } {}

		Transform2D & operator=(Transform const & value) { return (Transform2D &)Transform::operator=(value); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_TRANSFORM_2D_HPP_
