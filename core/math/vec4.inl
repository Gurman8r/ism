#ifdef _ISM_MATRIX_HPP_

/* VEC4 */

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> inline TVec4<T> operator-(TVec4<T> const & a) noexcept { return { -a[0], -a[1], -a[2], -a[3] }; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> NODISCARD TVec4<T> asin(TVec4<T> const & a) noexcept
	{
		return { ism::asin(a[0]), ism::asin(a[1]), ism::asin(a[2]), ism::asin(a[3]) };
	}

	template <class T
	> NODISCARD TVec4<T> acos(TVec4<T> const & a) noexcept
	{
		return { ism::acos(a[0]), ism::acos(a[1]), ism::acos(a[2]), ism::acos(a[3]) };
	}

	template <class T
	> NODISCARD TVec4<T> atan(TVec4<T> const & a) noexcept
	{
		return { ism::atan(a[0]), ism::atan(a[1]), ism::atan(a[2]), ism::atan(a[3]) };
	}

	template <class T
	> NODISCARD TVec4<T> sin(TVec4<T> const & a) noexcept
	{
		return { ism::sin(a[0]), ism::sin(a[1]), ism::sin(a[2]), ism::sin(a[3]) };
	}

	template <class T
	> NODISCARD TVec4<T> cos(TVec4<T> const & a) noexcept
	{
		return { ism::cos(a[0]), ism::cos(a[1]), ism::cos(a[2]), ism::cos(a[3]) };
	}

	template <class T
	> NODISCARD TVec4<T> tan(TVec4<T> const & a) noexcept
	{
		return { ism::tan(a[0]), ism::tan(a[1]), ism::tan(a[2]), ism::tan(a[3]) };
	}

	template <class T
	> NODISCARD TVec4<T> sqrt(TVec4<T> const & a) noexcept
	{
		return { ism::sqrt(a[0]), ism::sqrt(a[1]), ism::sqrt(a[2]), ism::sqrt(a[3]) };
	}

	template <class T
	> NODISCARD TVec4<T> inversesqrt(TVec4<T> const & a) noexcept
	{
		return { ism::inversesqrt(a[0]), ism::inversesqrt(a[1]), ism::inversesqrt(a[2]), ism::inversesqrt(a[3]) };
	}

	template <class T
	> NODISCARD TVec4<T> abs(TVec4<T> const & a, TVec4<T> const & b) noexcept
	{
		return { ism::abs(a[0]), ism::abs(a[1]), ism::abs(a[2]), ism::abs(a[3]) };
	}

	template <class T
	> NODISCARD TVec4<T> pow(TVec4<T> const & a, T const b) noexcept
	{
		return { ism::pow(a[0], b), ism::pow(a[1], b), ism::pow(a[2], b), ism::pow(a[3], b) };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> NODISCARD T dot(TVec4<T> const & a, TVec4<T> const & b) noexcept
	{
		return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class A, class B
	> constexpr bool operator==(TVec4<A> const & a, TVec4<B> const & b) noexcept
	{
		return (std::addressof(a) == std::addressof(b)) || (a[0] == (A)b[0] && a[1] == (A)b[1] && a[2] == (A)b[2] && a[3] == (A)b[3]);
	}

	template <class A, class B
	> constexpr bool operator!=(TVec4<A> const & a, TVec4<B> const & b) noexcept
	{
		return !(a == b);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class A, class B
	> constexpr TVec4<A> & operator+=(TVec4<A> & a, TVec4<B> const & b) noexcept
	{
		a[0] += (A)b[0];
		a[1] += (A)b[1];
		a[2] += (A)b[2];
		a[3] += (A)b[3];
		return a;
	}

	template <class A, class B
	> constexpr TVec4<A> & operator-=(TVec4<A> & a, TVec4<B> const & b) noexcept
	{
		a[0] -= (A)b[0];
		a[1] -= (A)b[1];
		a[2] -= (A)b[2];
		a[3] -= (A)b[3];
		return a;
	}

	template <class A, class B
	> constexpr TVec4<A> & operator*=(TVec4<A> & a, TVec4<B> const & b) noexcept
	{
		a[0] *= (A)b[0];
		a[1] *= (A)b[1];
		a[2] *= (A)b[2];
		a[3] *= (A)b[3];
		return a;
	}

	template <class A, class B
	> constexpr TVec4<A> & operator/=(TVec4<A> & a, TVec4<B> const & b) noexcept
	{
		a[0] /= (A)b[0];
		a[1] /= (A)b[1];
		a[2] /= (A)b[2];
		a[3] /= (A)b[3];
		return a;
	}

	template <class A, class B
	> constexpr TVec4<A> & operator%=(TVec4<A> & a, TVec4<B> const & b) noexcept
	{
		a[0] %= (A)b[0];
		a[1] %= (A)b[1];
		a[2] %= (A)b[2];
		a[3] %= (A)b[3];
		return a;
	}

	template <class A, class B
	> constexpr TVec4<A> & operator^=(TVec4<A> & a, TVec4<B> const & b) noexcept
	{
		a[0] ^= (A)b[0];
		a[1] ^= (A)b[1];
		a[2] ^= (A)b[2];
		a[3] ^= (A)b[3];
		return a;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec4<A> & operator+=(TVec4<A> & a, B const b) noexcept
	{
		a[0] += (A)b;
		a[1] += (A)b;
		a[2] += (A)b;
		a[3] += (A)b;
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec4<A> & operator-=(TVec4<A> & a, B const b) noexcept
	{
		a[0] -= (A)b;
		a[1] -= (A)b;
		a[2] -= (A)b;
		a[3] -= (A)b;
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec4<A> & operator*=(TVec4<A> & a, B const b) noexcept
	{
		a[0] *= (A)b;
		a[1] *= (A)b;
		a[2] *= (A)b;
		a[3] *= (A)b;
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec4<A> & operator/=(TVec4<A> & a, B const b) noexcept
	{
		a[0] /= (A)b;
		a[1] /= (A)b;
		a[1] /= (A)b;
		a[3] /= (A)b;
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec4<A> & operator%=(TVec4<A> & a, B const b) noexcept
	{
		a[0] %= (A)b;
		a[1] %= (A)b;
		a[2] %= (A)b;
		a[3] %= (A)b;
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec4<A> & operator^=(TVec4<A> & a, B const b) noexcept
	{
		a[0] ^= (A)b;
		a[1] ^= (A)b;
		a[2] ^= (A)b;
		a[3] ^= (A)b;
		return a;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // _ISM_MATRIX_HPP_
