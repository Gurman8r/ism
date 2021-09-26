#ifndef _ISM_VECTOR_HPP_
#define _ISM_VECTOR_HPP_

#include <core/os/memory.hpp>

#include <forward_list>
#include <vector>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// vector
	template <class _Ty
	> ALIAS(Vector) std::vector<_Ty, PolymorphicAllocator<_Ty>>;

	// forward_list
	template <class _Ty
	> ALIAS(ForwardList) std::forward_list<_Ty, PolymorphicAllocator<_Ty>>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class _Ty, class T
	> NODISCARD bool has(Vector<_Ty> const & l, T && value)
	{
		return l.end() != std::find(l.begin(), l.end(), FWD(value));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	// any container
	template <class T> class AnyContainer
	{
		Vector<T> v{};

	public:
		AnyContainer() = default;

		template <class It, class = std::enable_if_t<mpl::is_input_iterator_v<It>>
		> AnyContainer(It first, It last) : v{ first, last } {}

		template <class Container, class = std::enable_if_t<std::is_convertible_v<decltype(*std::begin(std::declval<Container const &>())), T>>
		> AnyContainer(Container const & c) : AnyContainer{ std::begin(c), std::end(c) } {}

		template <class U, class = std::enable_if_t<std::is_convertible_v<U, T>>
		> AnyContainer(std::initializer_list<U> const & c) : AnyContainer{ c.begin(), c.end() } {}

		AnyContainer(Vector<T> && v) : v{ std::move(v) } {}

		operator Vector<T> && () && { return std::move(v); }

		Vector<T> & operator*() { return v; }
		
		Vector<T> * operator->() { return &v; }

		Vector<T> const & operator*() const { return v; }
		
		Vector<T> const * operator->() const { return &v; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_VECTOR_HPP_
