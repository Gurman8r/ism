#ifdef _ISM_MATRIX_HPP_

/* VEC3 */

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> inline TVec3<T> operator-(TVec3<T> const & a) noexcept { return { -a[0], -a[1], -a[2] }; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> NODISCARD TVec3<T> asin(TVec3<T> const & a) noexcept
	{
		return { ism::asin(a[0]), ism::asin(a[1]), ism::asin(a[2]) };
	}

	template <class T
	> NODISCARD TVec3<T> acos(TVec3<T> const & a) noexcept
	{
		return { ism::acos(a[0]), ism::acos(a[1]), ism::acos(a[2]) };
	}

	template <class T
	> NODISCARD TVec3<T> atan(TVec3<T> const & a) noexcept
	{
		return { ism::atan(a[0]), ism::atan(a[1]), ism::atan(a[2]) };
	}

	template <class T
	> NODISCARD TVec3<T> sin(TVec3<T> const & a) noexcept
	{
		return { ism::sin(a[0]), ism::sin(a[1]), ism::sin(a[2]) };
	}

	template <class T
	> NODISCARD TVec3<T> cos(TVec3<T> const & a) noexcept
	{
		return { ism::cos(a[0]), ism::cos(a[1]), ism::cos(a[2]) };
	}

	template <class T
	> NODISCARD TVec3<T> tan(TVec3<T> const & a) noexcept
	{
		return { ism::tan(a[0]), ism::tan(a[1]), ism::tan(a[2]) };
	}

	template <class T
	> NODISCARD TVec3<T> sqrt(TVec3<T> const & a) noexcept
	{
		return { ism::sqrt(a[0]), ism::sqrt(a[1]), ism::sqrt(a[2]) };
	}

	template <class T
	> NODISCARD TVec3<T> inversesqrt(TVec3<T> const & a) noexcept
	{
		return { ism::inversesqrt(a[0]), ism::inversesqrt(a[1]), ism::inversesqrt(a[2]) };
	}

	template <class T
	> NODISCARD TVec3<T> abs(TVec3<T> const & a) noexcept
	{
		return { ism::abs(a[0]), ism::abs(a[1]), ism::abs(a[2]) };
	}

	template <class T
	> NODISCARD TVec3<T> pow(TVec3<T> const & a, T const b) noexcept
	{
		return { ism::pow(a[0], b), ism::pow(a[1], b), ism::pow(a[2], b) };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> NODISCARD T dot(TVec3<T> const & a, TVec3<T> const & b) noexcept
	{
		return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class A, class B
	> constexpr bool operator==(TVec3<A> const & a, TVec3<B> const & b) noexcept
	{
		return (std::addressof(a) == std::addressof(b)) || (a[0] == (A)b[0] && a[1] == (A)b[1] && a[2] == (A)b[2]);
	}

	template <class A, class B
	> constexpr bool operator!=(TVec3<A> const & a, TVec3<B> const & b) noexcept
	{
		return !(a == b);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr bool operator==(TVec3<A> const & a, B const b) noexcept
	{
		return (a[0] == (A)b && a[1] == (A)b && a[2] == (A)b);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class A, class B
	> constexpr TVec3<A> & operator+=(TVec3<A> & a, TVec3<B> const & b) noexcept
	{
		a[0] += (A)b[0];
		a[1] += (A)b[1];
		a[2] += (A)b[2];
		return a;
	}

	template <class A, class B
	> constexpr TVec3<A> & operator-=(TVec3<A> & a, TVec3<B> const & b) noexcept
	{
		a[0] -= (A)b[0];
		a[1] -= (A)b[1];
		a[2] -= (A)b[2];
		return a;
	}

	template <class A, class B
	> constexpr TVec3<A> & operator*=(TVec3<A> & a, TVec3<B> const & b) noexcept
	{
		a[0] *= (A)b[0];
		a[1] *= (A)b[1];
		a[2] *= (A)b[2];
		return a;
	}

	template <class A, class B
	> constexpr TVec3<A> & operator/=(TVec3<A> & a, TVec3<B> const & b) noexcept
	{
		a[0] /= (A)b[0];
		a[1] /= (A)b[1];
		a[2] /= (A)b[2];
		return a;
	}

	template <class A, class B
	> constexpr TVec3<A> & operator%=(TVec3<A> & a, TVec3<B> const & b) noexcept
	{
		a[0] %= (A)b[0];
		a[1] %= (A)b[1];
		a[2] %= (A)b[2];
		return a;
	}

	template <class A, class B
	> constexpr TVec3<A> & operator^=(TVec3<A> & a, TVec3<B> const & b) noexcept
	{
		a[0] ^= (A)b[0];
		a[1] ^= (A)b[1];
		a[2] ^= (A)b[2];
		return a;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec3<A> & operator+=(TVec3<A> & a, B const b) noexcept
	{
		a[0] += (A)b;
		a[1] += (A)b;
		a[2] += (A)b;
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec3<A> & operator-=(TVec3<A> & a, B const b) noexcept
	{
		a[0] -= (A)b;
		a[1] -= (A)b;
		a[2] -= (A)b;
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec3<A> & operator*=(TVec3<A> & a, B const b) noexcept
	{
		a[0] *= (A)b;
		a[1] *= (A)b;
		a[2] *= (A)b;
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec3<A> & operator/=(TVec3<A> & a, B const b) noexcept
	{
		a[0] /= (A)b;
		a[1] /= (A)b;
		a[1] /= (A)b;
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec3<A> & operator%=(TVec3<A> & a, B const b) noexcept
	{
		a[0] %= (A)b;
		a[1] %= (A)b;
		a[2] %= (A)b;
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec3<A> & operator^=(TVec3<A> & a, B const b) noexcept
	{
		a[0] ^= (A)b;
		a[1] ^= (A)b;
		a[2] ^= (A)b;
		return a;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // _ISM_MATRIX_HPP_
