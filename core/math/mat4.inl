#ifdef _ISM_MATRIX_HPP_

/* MAT4 */

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class A, class B
	> constexpr bool operator==(TMat4<A> const & a, TMat4<B> const & b) noexcept {
		return (std::addressof(a) == std::addressof(b)) || (
			a[0] == (A)b[0] &&
			a[1] == (A)b[1] &&
			a[2] == (A)b[2] &&
			a[3] == (A)b[3] &&
			a[4] == (A)b[4] &&
			a[5] == (A)b[5] &&
			a[6] == (A)b[6] &&
			a[7] == (A)b[7] &&
			a[8] == (A)b[8] &&
			a[9] == (A)b[9] &&
			a[10] == (A)b[10] &&
			a[11] == (A)b[11] &&
			a[12] == (A)b[12] &&
			a[13] == (A)b[13] &&
			a[14] == (A)b[14] &&
			a[15] == (A)b[15]);
	}

	template <class A, class B
	> constexpr bool operator!=(TMat4<A> const & a, TMat4<B> const & b) noexcept {
		return !(a == b);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TMat4<A> & operator+=(TMat4<A> & a, B const b) noexcept {
		for (auto & e : a) { e += (A)b; }
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TMat4<A> & operator-=(TMat4<A> & a, B const b) noexcept {
		for (auto & e : a) { e -= (A)b; }
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TMat4<A> & operator*=(TMat4<A> & a, B const b) noexcept {
		for (auto & e : a) { e *= (A)b; }
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TMat4<A> & operator/=(TMat4<A> & a, B const b) noexcept {
		for (auto & e : a) { e /= (A)b; }
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TMat4<A> & operator%=(TMat4<A> & a, B const b) noexcept {
		for (auto & e : a) { e %= (A)b; }
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TMat4<A> & operator^=(TMat4<A> & a, B const b) noexcept {
		for (auto & e : a) { e ^= (A)b; }
		return a;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // _ISM_MATRIX_HPP_
