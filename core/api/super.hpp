#ifndef _ISM_SUPER_HPP_
#define _ISM_SUPER_HPP_

#include <core/typedefs.hpp>
#include <core/string/path.hpp>
#include <core/math/color.hpp>
#include <core/math/quat.hpp>
#include <core/math/transform.hpp>
#include <core/math/transform_2d.hpp>
#include <core/math/vector4.hpp>
#include <core/templates/any.hpp>
#include <core/templates/atomic.hpp>
#include <core/templates/ecs.hpp>
#include <core/templates/flat_map.hpp>
#include <core/templates/hash_map.hpp>
#include <core/templates/hash_set.hpp>
#include <core/templates/map.hpp>
#include <core/templates/set.hpp>
#include <core/templates/type_info.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Reference;
	class Resource;

	DECL_HANDLE(InstanceID);

	template <class T> class Ref;
	ALIAS(REF) Ref<Reference>;
	ALIAS(RES) Ref<Resource>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class To, class From
	> NODISCARD auto super_cast(From from)
	{
		using T = mpl::intrinsic_t<To>;
		using F = mpl::intrinsic_t<From>;

		static_assert(!std::is_same_v<T, F>, "?");

		if constexpr (std::is_same_v<To, std::add_const_t<T>>)
		{
			return dynamic_cast<To const *>(const_cast<F const *>(from));
		}
		else
		{
			static_assert(std::is_same_v<To, T>, "?");

			return dynamic_cast<To *>(const_cast<F *>(from));
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_SUPER_HPP_
