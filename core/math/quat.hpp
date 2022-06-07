#ifndef _ISM_QUAT_HPP_
#define _ISM_QUAT_HPP_

#include <core/math/vector4.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct Quat : Vec4
	{
		using Vec4::Vec4;

		using Vec4::operator=;

		explicit Quat(glm::quat const & q) : Vec4{ q.x, q.y, q.z, q.w } {}

		Quat & operator=(glm::quat const & q) { Quat temp{ q }; return (Quat &)swap(temp); }

		operator glm::quat & () const noexcept { return *((glm::quat *)(void *)this); }

		operator glm::quat const & () const noexcept { return *((glm::quat const *)(void *)this); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_QUAT_HPP_
