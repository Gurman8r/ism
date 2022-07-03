#ifndef _ISM_SET_HPP_
#define _ISM_SET_HPP_

#include <core/os/memory.hpp>

#include <set>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// set base
	template <class T, class Pr = Less<T>
	> ALIAS(_SetBase) std::set<T, Pr, PolymorphicAllocator<T>>;

	// set
	template <class T, class Pr = Less<T>
	> class Set : public _SetBase<T, Pr> {
	public:
		using base_type = typename _SetBase<T, Pr>;
		using base_type::base_type;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// multi set base
	template <class T, class Pr = Less<T>
	> ALIAS(_MultiSetBase) std::multiset<T, Pr, PolymorphicAllocator<T>>;

	// multi set
	template <class T, class Pr = Less<T>
	> class MultiSet : public _MultiSetBase<T, Pr> {
	public:
		using base_type = typename _MultiSetBase<T, Pr>;
		using base_type::base_type;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_SET_HPP_
