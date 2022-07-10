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

		NODISCARD operator void * () const noexcept { return !empty() ? (void *)data() : nullptr; }

		NODISCARD auto find(T const & value) { return std::find(begin(), end(), value); }
		NODISCARD auto find(T && value) noexcept { return std::find(begin(), end(), FWD(value)); }

		NODISCARD auto find(T const & value) const { return std::find(begin(), end(), value); }
		NODISCARD auto find(T && value) const noexcept { return std::find(begin(), end(), FWD(value)); }

		NODISCARD bool contains(T const & value) const { return end() != find(value); }
		NODISCARD bool contains(T && value) const noexcept { return end() != find(FWD(value)); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// getptr
	template <class V, class K
	> auto getptr(List<V> const & l, K && value)
	{
		if (auto const it{ l.find(FWD(value)) }; it != l.end())
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
