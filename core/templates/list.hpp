#ifndef _ISM_LIST_HPP_
#define _ISM_LIST_HPP_

#include <core/os/memory.hpp>

#include <vector>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// list base
	template <class T
	> ALIAS(_ListBase) std::vector<T, PolymorphicAllocator<T>>;

	// list
	template <class T
	> class List : public _ListBase<T>
	{
	public:
		using base_type = _ListBase<T>;
		using base_type::base_type;
		using base_type::operator=;

		NODISCARD bool contains(T const & value) const { return end() != std::find(begin(), end(), value); }
		NODISCARD bool contains(T && value) const noexcept { return end() != std::find(begin(), end(), FWD(value)); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// getptr
	template <class V, class K
	> auto getptr(List<V> const & l, K && value)
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

#endif // !_ISM_LIST_HPP_
