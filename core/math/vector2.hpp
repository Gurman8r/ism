#ifndef _ISM_VECTOR2_HPP_
#define _ISM_VECTOR2_HPP_

#include <core/math/maths.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> struct Vector2D : Matrix<T, 2, 1>
	{
		using self_type = typename Vector2D<T>;
		using base_type = typename Matrix<T, 2, 1>;
		using typename base_type::value_type;
		using base_type::Matrix;
		using base_type::operator=;

		explicit Vector2D(glm::tvec2<T> const & q) : base_type{ q.x, q.y, q.z, q.w } {}
		NODISCARD Vector2D & operator=(glm::tvec2<T> const & q) { Vector2D temp{ q }; return (Vector2D &)swap(temp); }
		NODISCARD operator glm::tvec2<T> & () const noexcept { return *((glm::tvec2<T> *)(void *)this); }
		NODISCARD operator glm::tvec2<T> const & () const noexcept { return *((glm::tvec2<T> const *)(void *)this); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_VECTOR2_HPP_
