#ifdef _ISM_MATRIX_HPP_

/* VEC4 */

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class A, class B
	> constexpr bool operator==(TVec4<A> const & a, TVec4<B> const & b) noexcept {
		return (std::addressof(a) == std::addressof(b)) || (
			a[0] == (A)b[0] &&
			a[1] == (A)b[1] &&
			a[2] == (A)b[2] &&
			a[3] == (A)b[3]);
	}

	template <class A, class B
	> constexpr bool operator!=(TVec4<A> const & a, TVec4<B> const & b) noexcept {
		return !(a == b);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class A, class B
	> constexpr TVec4<A> & operator+=(TVec4<A> & a, TVec4<B> const & b) noexcept {
		a[0] += (A)b[0];
		a[1] += (A)b[1];
		a[2] += (A)b[2];
		a[3] += (A)b[3];
		return a;
	}

	template <class A, class B
	> constexpr TVec4<A> & operator-=(TVec4<A> & a, TVec4<B> const & b) noexcept {
		a[0] -= (A)b[0];
		a[1] -= (A)b[1];
		a[2] -= (A)b[2];
		a[3] -= (A)b[3];
		return a;
	}

	template <class A, class B
	> constexpr TVec4<A> & operator*=(TVec4<A> & a, TVec4<B> const & b) noexcept {
		a[0] *= (A)b[0];
		a[1] *= (A)b[1];
		a[2] *= (A)b[2];
		a[3] *= (A)b[3];
		return a;
	}

	template <class A, class B
	> constexpr TVec4<A> & operator/=(TVec4<A> & a, TVec4<B> const & b) noexcept {
		a[0] /= (A)b[0];
		a[1] /= (A)b[1];
		a[2] /= (A)b[2];
		a[3] /= (A)b[3];
		return a;
	}

	template <class A, class B
	> constexpr TVec4<A> & operator%=(TVec4<A> & a, TVec4<B> const & b) noexcept {
		a[0] %= (A)b[0];
		a[1] %= (A)b[1];
		a[2] %= (A)b[2];
		a[3] %= (A)b[3];
		return a;
	}

	template <class A, class B
	> constexpr TVec4<A> & operator^=(TVec4<A> & a, TVec4<B> const & b) noexcept {
		a[0] ^= (A)b[0];
		a[1] ^= (A)b[1];
		a[2] ^= (A)b[2];
		a[3] ^= (A)b[3];
		return a;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec4<A> & operator+=(TVec4<A> & a, B const b) noexcept {
		a[0] += (A)b;
		a[1] += (A)b;
		a[2] += (A)b;
		a[3] += (A)b;
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec4<A> & operator-=(TVec4<A> & a, B const b) noexcept {
		a[0] -= (A)b;
		a[1] -= (A)b;
		a[2] -= (A)b;
		a[3] -= (A)b;
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec4<A> & operator*=(TVec4<A> & a, B const b) noexcept {
		a[0] *= (A)b;
		a[1] *= (A)b;
		a[2] *= (A)b;
		a[3] *= (A)b;
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec4<A> & operator/=(TVec4<A> & a, B const b) noexcept {
		a[0] /= (A)b;
		a[1] /= (A)b;
		a[1] /= (A)b;
		a[3] /= (A)b;
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec4<A> & operator%=(TVec4<A> & a, B const b) noexcept {
		a[0] %= (A)b;
		a[1] %= (A)b;
		a[2] %= (A)b;
		a[3] %= (A)b;
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec4<A> & operator^=(TVec4<A> & a, B const b) noexcept {
		a[0] ^= (A)b;
		a[1] ^= (A)b;
		a[2] ^= (A)b;
		a[3] ^= (A)b;
		return a;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // _ISM_MATRIX_HPP_
