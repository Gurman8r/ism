#ifdef _ISM_MATRIX_HPP_

/* MAT2 */

namespace ism
{
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
