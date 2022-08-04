#ifdef _ISM_MATRIX_HPP_

/* VEC3 */

namespace ism::util
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class A, class B
	> NODISCARD TVec3<A> cross(TVec3<A> const & a, TVec3<B> const & b) noexcept
	{
		return {
			a[1] * (A)b[2] - a[2] * (A)b[1],
			a[2] * (A)b[0] - a[0] * (A)b[2],
			a[0] * (A)b[1] - a[1] * (A)b[0],
		};
	}

	template <class A, class B
	> NODISCARD A dot(TVec3<A> const & a, TVec3<B> const & b) noexcept
	{
		return a[0] * (A)b[0] + a[1] * (A)b[1] + a[2] * (A)b[2];
	}

	template <class A
	> NODISCARD TVec3<A> normalize(TVec3<A> const & a) noexcept
	{
		glm::mat4() * glm::vec4();

		A const il{ (A)1 / (std::sqrtf(dot(a, a)) + FLT_EPSILON) };
		return {
			a[0] * il,
			a[1] * il,
			a[2] * il,
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class A, class B
	> constexpr bool operator==(TVec3<A> const & a, TVec3<B> const & b) noexcept {
		return (std::addressof(a) == std::addressof(b)) || (
			a[0] == (A)b[0] &&
			a[1] == (A)b[1] &&
			a[2] == (A)b[2]);
	}

	template <class A, class B
	> constexpr bool operator!=(TVec3<A> const & a, TVec3<B> const & b) noexcept {
		return !(a == b);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class A, class B
	> constexpr TVec3<A> & operator+=(TVec3<A> & a, TVec3<B> const & b) noexcept {
		a[0] += (A)b[0];
		a[1] += (A)b[1];
		a[2] += (A)b[2];
		return a;
	}

	template <class A, class B
	> constexpr TVec3<A> & operator-=(TVec3<A> & a, TVec3<B> const & b) noexcept {
		a[0] -= (A)b[0];
		a[1] -= (A)b[1];
		a[2] -= (A)b[2];
		return a;
	}

	template <class A, class B
	> constexpr TVec3<A> & operator*=(TVec3<A> & a, TVec3<B> const & b) noexcept {
		a[0] *= (A)b[0];
		a[1] *= (A)b[1];
		a[2] *= (A)b[2];
		return a;
	}

	template <class A, class B
	> constexpr TVec3<A> & operator/=(TVec3<A> & a, TVec3<B> const & b) noexcept {
		a[0] /= (A)b[0];
		a[1] /= (A)b[1];
		a[2] /= (A)b[2];
		return a;
	}

	template <class A, class B
	> constexpr TVec3<A> & operator%=(TVec3<A> & a, TVec3<B> const & b) noexcept {
		a[0] %= (A)b[0];
		a[1] %= (A)b[1];
		a[2] %= (A)b[2];
		return a;
	}

	template <class A, class B
	> constexpr TVec3<A> & operator^=(TVec3<A> & a, TVec3<B> const & b) noexcept {
		a[0] ^= (A)b[0];
		a[1] ^= (A)b[1];
		a[2] ^= (A)b[2];
		return a;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec3<A> & operator+=(TVec3<A> & a, B const b) noexcept {
		a[0] += (A)b;
		a[1] += (A)b;
		a[2] += (A)b;
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec3<A> & operator-=(TVec3<A> & a, B const b) noexcept {
		a[0] -= (A)b;
		a[1] -= (A)b;
		a[2] -= (A)b;
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec3<A> & operator*=(TVec3<A> & a, B const b) noexcept {
		a[0] *= (A)b;
		a[1] *= (A)b;
		a[2] *= (A)b;
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec3<A> & operator/=(TVec3<A> & a, B const b) noexcept {
		a[0] /= (A)b;
		a[1] /= (A)b;
		a[1] /= (A)b;
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec3<A> & operator%=(TVec3<A> & a, B const b) noexcept {
		a[0] %= (A)b;
		a[1] %= (A)b;
		a[2] %= (A)b;
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec3<A> & operator^=(TVec3<A> & a, B const b) noexcept {
		a[0] ^= (A)b;
		a[1] ^= (A)b;
		a[2] ^= (A)b;
		return a;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // _ISM_MATRIX_HPP_
