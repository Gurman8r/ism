#ifdef _ISM_MATRIX_HPP_

/* VEC2 */

namespace ism::util
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> NODISCARD T dot(TVec2<T> const & a, TVec2<T> const & b) noexcept
	{
		return a[0] * b[0] + a[1] * b[1];
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class A, class B
	> constexpr bool operator==(TVec2<A> const & a, TVec2<B> const & b) noexcept {
		return (std::addressof(a) == std::addressof(b)) || (
			a[0] == (A)b[0] &&
			a[1] == (A)b[1]);
	}

	template <class A, class B
	> constexpr bool operator!=(TVec2<A> const & a, TVec2<B> const & b) noexcept {
		return !(a == b);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class A, class B
	> constexpr TVec2<A> & operator+=(TVec2<A> & a, TVec2<B> const & b) noexcept {
		a[0] += (A)b[0];
		a[1] += (A)b[1];
		return a;
	}

	template <class A, class B
	> constexpr TVec2<A> & operator-=(TVec2<A> & a, TVec2<B> const & b) noexcept {
		a[0] -= (A)b[0];
		a[1] -= (A)b[1];
		return a;
	}

	template <class A, class B
	> constexpr TVec2<A> & operator*=(TVec2<A> & a, TVec2<B> const & b) noexcept {
		a[0] *= (A)b[0];
		a[1] *= (A)b[1];
		return a;
	}

	template <class A, class B
	> constexpr TVec2<A> & operator/=(TVec2<A> & a, TVec2<B> const & b) noexcept {
		a[0] /= (A)b[0];
		a[1] /= (A)b[1];
		return a;
	}

	template <class A, class B
	> constexpr TVec2<A> & operator%=(TVec2<A> & a, TVec2<B> const & b) noexcept {
		a[0] %= (A)b[0];
		a[1] %= (A)b[1];
		return a;
	}

	template <class A, class B
	> constexpr TVec2<A> & operator^=(TVec2<A> & a, TVec2<B> const & b) noexcept {
		a[0] ^= (A)b[0];
		a[1] ^= (A)b[1];
		return a;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec2<A> & operator+=(TVec2<A> & a, B const b) noexcept {
		a[0] += (A)b;
		a[1] += (A)b;
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec2<A> & operator-=(TVec2<A> & a, B const b) noexcept {
		a[0] -= (A)b;
		a[1] -= (A)b;
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec2<A> & operator*=(TVec2<A> & a, B const b) noexcept {
		a[0] *= (A)b;
		a[1] *= (A)b;
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec2<A> & operator/=(TVec2<A> & a, B const b) noexcept {
		a[0] /= (A)b;
		a[1] /= (A)b;
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec2<A> & operator%=(TVec2<A> & a, B const b) noexcept {
		a[0] %= (A)b;
		a[1] %= (A)b;
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec2<A> & operator^=(TVec2<A> & a, B const b) noexcept {
		a[0] ^= (A)b;
		a[1] ^= (A)b;
		return a;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // _ISM_MATRIX_HPP_
