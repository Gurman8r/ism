#ifdef _ISM_MATRIX_HPP_

/* MAT2 */

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class A, class B
	> constexpr bool operator==(TMat2<A> const & a, TMat2<B> const & b) noexcept {
		return (std::addressof(a) == std::addressof(b)) || (
			a[0] == (A)b[0] &&
			a[1] == (A)b[1] &&
			a[2] == (A)b[2] &&
			a[3] == (A)b[3]);
	}

	template <class A, class B
	> constexpr bool operator!=(TMat2<A> const & a, TMat2<B> const & b) noexcept {
		return !(a == b);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TMat2<A> & operator+=(TMat2<A> & a, B const b) noexcept {
		for (auto & e : a) { e += (A)b; }
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TMat2<A> & operator-=(TMat2<A> & a, B const b) noexcept {
		for (auto & e : a) { e -= (A)b; }
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TMat2<A> & operator*=(TMat2<A> & a, B const b) noexcept {
		for (auto & e : a) { e *= (A)b; }
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TMat2<A> & operator/=(TMat2<A> & a, B const b) noexcept {
		for (auto & e : a) { e /= (A)b; }
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TMat2<A> & operator%=(TMat2<A> & a, B const b) noexcept {
		for (auto & e : a) { e %= (A)b; }
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TMat2<A> & operator^=(TMat2<A> & a, B const b) noexcept {
		for (auto & e : a) { e ^= (A)b; }
		return a;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // _ISM_MATRIX_HPP_
