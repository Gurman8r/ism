#ifndef _ISM_VECTOR4_HPP_
#define _ISM_VECTOR4_HPP_

#include <core/math/vector3.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> struct Vector4D : Matrix<T, 4, 1>
	{
		using self_type = typename Vector4D<T>;
		using base_type = typename Matrix<T, 4, 1>;
		using typename base_type::value_type;
		using base_type::Matrix;
		using base_type::operator=;

		explicit Vector4D(glm::tvec4<T> const & q) : base_type{ q.x, q.y, q.z, q.w } {}
		NODISCARD self_type & operator=(glm::tvec4<T> const & q) { Vector4D temp{ q }; return (Vector4D &)swap(temp); }
		NODISCARD operator glm::tvec4<T> & () const noexcept { return *((glm::tvec4<T> *)(void *)this); }
		NODISCARD operator glm::tvec4<T> const & () const noexcept { return *((glm::tvec4<T> const *)(void *)this); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_VECTOR4_HPP_
