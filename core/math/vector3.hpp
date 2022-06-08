#ifndef _ISM_VECTOR3_HPP_
#define _ISM_VECTOR3_HPP_

#include <core/math/vector2.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> struct Vector3D : Matrix<T, 3, 1>
	{
		using self_type = typename Vector3D<T>;
		using base_type = typename Matrix<T, 3, 1>;
		using typename base_type::value_type;
		using base_type::Matrix;
		using base_type::operator=;

		explicit Vector3D(glm::tvec3<T> const & q) : base_type{ q.x, q.y, q.z, q.w } {}
		NODISCARD self_type & operator=(glm::tvec3<T> const & q) { Vector3D temp{ q }; return (Vector3D &)swap(temp); }
		NODISCARD operator glm::tvec3<T> & () const noexcept { return *((glm::tvec3<T> *)(void *)this); }
		NODISCARD operator glm::tvec3<T> const & () const noexcept { return *((glm::tvec3<T> const *)(void *)this); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_VECTOR3_HPP_
