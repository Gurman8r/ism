#ifndef _ISM_QUAT_HPP_
#define _ISM_QUAT_HPP_

#include <core/math/vector4.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct Quat : Matrix<float_t, 4, 1>
	{
		using self_type = typename Quat;
		using base_type = typename Matrix<float_t, 4, 1>;
		using typename base_type::value_type;
		using base_type::Matrix;
		using base_type::operator=;

		explicit Quat(glm::quat const & q) : base_type{ q.x, q.y, q.z, q.w } {}
		NODISCARD self_type & operator=(glm::quat const & q) { Quat temp{ q }; return (Quat &)swap(temp); }
		NODISCARD operator glm::quat & () const noexcept { return *((glm::quat *)(void *)this); }
		NODISCARD operator glm::quat const & () const noexcept { return *((glm::quat const *)(void *)this); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_QUAT_HPP_
