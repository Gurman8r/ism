#ifndef _ISM_VECTOR_HPP_
#define _ISM_VECTOR_HPP_

#include <core/os/memory.hpp>

#include <vector>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// vector base
	template <class T
	> ALIAS(_VectorBase) std::vector<T, PolymorphicAllocator<T>>;

	// vector
	template <class T
	> class Vector : public _VectorBase<T>
	{
	public:
		using base_type = _VectorBase<T>;
		using base_type::base_type;
		using base_type::operator=;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// has
	template <class V, class K
	> bool has(Vector<V> const & l, K && value)
	{
		return l.end() != std::find(l.begin(), l.end(), FWD(value));
	}

	// getptr
	template <class V, class K
	> auto getptr(Vector<V> const & l, K && value)
	{
		if (auto const it{ std::find(l.begin(), l.end(), FWD(value)) }; it != l.end())
		{
			return (V *)(void *)std::addressof(*it);
		}
		else
		{
			return nullptr;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_VECTOR_HPP_
