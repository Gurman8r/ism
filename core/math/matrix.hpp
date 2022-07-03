#ifndef _ISM_MATRIX_HPP_
#define _ISM_MATRIX_HPP_

#include <core/templates/array.hpp>

// TYPES
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Matrix
	template <class _T, size_t _Width, size_t _Height
	> struct Matrix;

	// Matrix<T, N, N>
	template <class T, size_t N
	> ALIAS(TMatrixNxN) Matrix<T, N, N>;

	// Matrix<T, N, 1>
	template <class T, size_t N
	> ALIAS(TMatrixNx1) Matrix<T, N, 1>;

	// Matrix<T, 2, 2>
	template <class T
	> ALIAS(TMatrix2x2) TMatrixNxN<T, 2>;

	// Matrix<T, 3, 3>
	template <class T
	> ALIAS(TMatrix3x3) TMatrixNxN<T, 3>;

	// Matrix<T, 4 ,4>
	template <class T
	> ALIAS(TMatrix4x4) TMatrixNxN<T, 4>;

	// Matrix<T, 2, 1>
	template <class T
	> ALIAS(TVector2) TMatrixNx1<T, 2>;

	// Matrix<T, 3, 1>
	template <class T
	> ALIAS(TVector3) TMatrixNx1<T, 3>;

	// Matrix<T, 4, 1>
	template <class T
	> ALIAS(TVector4) TMatrixNx1<T, 4>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Mat2
	ALIAS(Mat2b) TMatrix2x2<uint8_t>;
	ALIAS(Mat2i) TMatrix2x2<int32_t>;
	ALIAS(Mat2u) TMatrix2x2<uint32_t>;
	ALIAS(Mat2f) TMatrix2x2<float32_t>;
	ALIAS(Mat2d) TMatrix2x2<float64_t>;
	ALIAS(Mat2s) TMatrix2x2<size_t>;
	ALIAS(Mat2) Mat2f;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Mat3
	ALIAS(Mat3b) TMatrix3x3<uint8_t>;
	ALIAS(Mat3i) TMatrix3x3<int32_t>;
	ALIAS(Mat3u) TMatrix3x3<uint32_t>;
	ALIAS(Mat3f) TMatrix3x3<float32_t>;
	ALIAS(Mat3d) TMatrix3x3<float64_t>;
	ALIAS(Mat3s) TMatrix3x3<size_t>;
	ALIAS(Mat3) Mat3f;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Mat4
	ALIAS(Mat4b) TMatrix4x4<uint8_t>;
	ALIAS(Mat4i) TMatrix4x4<int32_t>;
	ALIAS(Mat4u) TMatrix4x4<uint32_t>;
	ALIAS(Mat4f) TMatrix4x4<float32_t>;
	ALIAS(Mat4d) TMatrix4x4<float64_t>;
	ALIAS(Mat4s) TMatrix4x4<size_t>;
	ALIAS(Mat4) Mat4f;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Vec2
	ALIAS(Vec2b) TVector2<uint8_t>;
	ALIAS(Vec2i) TVector2<int32_t>;
	ALIAS(Vec2u) TVector2<uint32_t>;
	ALIAS(Vec2f) TVector2<float32_t>;
	ALIAS(Vec2d) TVector2<float64_t>;
	ALIAS(Vec2s) TVector2<size_t>;
	ALIAS(Vec2) Vec2f;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Vec3
	ALIAS(Vec3b) TVector3<uint8_t>;
	ALIAS(Vec3i) TVector3<int32_t>;
	ALIAS(Vec3u) TVector3<uint32_t>;
	ALIAS(Vec3f) TVector3<float32_t>;
	ALIAS(Vec3d) TVector3<float64_t>;
	ALIAS(Vec3s) TVector3<size_t>;
	ALIAS(Vec3) Vec3f;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Vec4
	ALIAS(Vec4b) TVector4<uint8_t>;
	ALIAS(Vec4i) TVector4<int32_t>;
	ALIAS(Vec4u) TVector4<uint32_t>;
	ALIAS(Vec4f) TVector4<float32_t>;
	ALIAS(Vec4d) TVector4<float64_t>;
	ALIAS(Vec4s) TVector4<size_t>;
	ALIAS(Vec4) Vec4f;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// MATRIX
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Matrix
	template <class _T, size_t _Width, size_t _Height
	> struct Matrix
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static_assert(0 < _Width, "Matrix width negative or zero");
		static_assert(0 < _Height, "Matrix height negative or zero");
		static_assert(std::is_scalar_v<_T>, "Matrix only supports scalar types");

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type				= typename _T;
		using self_type					= typename Matrix<value_type, _Width, _Height>;
		using storage_type				= typename Array<value_type, _Width * _Height>;
		using size_type					= typename storage_type::size_type;
		using difference_type			= typename storage_type::difference_type;
		using coord_type				= typename Array<size_type, 2>;
		using pointer					= typename storage_type::pointer;
		using reference					= typename storage_type::reference;
		using const_pointer				= typename storage_type::const_pointer;
		using const_reference			= typename storage_type::const_reference;
		using iterator					= typename storage_type::iterator;
		using const_iterator			= typename storage_type::const_iterator;
		using reverse_iterator			= typename storage_type::reverse_iterator;
		using const_reverse_iterator	= typename storage_type::const_reverse_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		storage_type m_data; // aggregate initializer

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr bool empty() noexcept { return false; }
		static constexpr auto height() noexcept -> size_t { return _Height; }
		static constexpr auto max_size() noexcept -> size_t { return _Width * _Height; }
		static constexpr auto size() noexcept -> size_t { return _Width * _Height; }
		static constexpr auto width() noexcept -> size_t { return _Width; }
		
		constexpr auto data() noexcept -> pointer { return m_data.data(); }
		constexpr auto data() const noexcept -> const_pointer { return m_data.data(); }

		constexpr operator storage_type & () & noexcept { return m_data; }
		constexpr operator storage_type const & () const & noexcept { return m_data; }
		constexpr operator storage_type && () && noexcept { return std::move(m_data); }
		constexpr operator pointer() noexcept { return m_data; }
		constexpr operator const_pointer() const noexcept { return m_data; }

		constexpr auto operator*() & noexcept -> reference { return (*m_data); }
		constexpr auto operator*() const & noexcept -> const_reference { return (*m_data); }
		constexpr auto operator*() && noexcept -> value_type && { return std::move(*m_data); }

		constexpr auto at(size_t const i) & noexcept -> reference { return m_data.at(i); }
		constexpr auto at(size_t const i) const & noexcept -> const_reference { return m_data.at(i); }
		constexpr auto at(size_t const i) && noexcept -> value_type && { return std::move(m_data.at(i)); }
		constexpr auto at(size_t const x, size_t const y) & noexcept -> reference { return at(y * _Width + x); }
		constexpr auto at(size_t const x, size_t const y) const & noexcept -> const_reference { return at(y * _Width + x); }
		constexpr auto at(size_t const x, size_t const y) && noexcept -> value_type && { return std::move(at(y * _Width + x)); }
		constexpr auto at(coord_type const & loc) & noexcept -> reference { return at(loc[0], loc[1]); }
		constexpr auto at(coord_type const & loc) const & noexcept -> const_reference { return at(loc[0], loc[1]); }
		constexpr auto at(coord_type const & loc) && noexcept -> value_type && { return std::move(at(loc[0], loc[1])); }

		constexpr auto back() & noexcept -> reference { return m_data.back(); }
		constexpr auto back() const & noexcept -> const_reference { return m_data.back(); }
		constexpr auto back() && noexcept -> value_type && { return std::move(m_data.back()); }
		constexpr auto front() & noexcept -> reference { return m_data.front(); }
		constexpr auto front() const & noexcept -> const_reference { return m_data.front(); }
		constexpr auto front() && noexcept -> value_type && { return std::move(m_data.front()); }

		constexpr auto begin() noexcept -> iterator { return m_data.begin(); }
		constexpr auto begin() const noexcept -> const_iterator { return m_data.begin(); }
		constexpr auto cbegin() const noexcept -> const_iterator { return m_data.cbegin(); }
		constexpr auto cend() const noexcept -> const_iterator { return m_data.cend(); }
		constexpr auto crbegin() const noexcept -> const_reverse_iterator { return m_data.crbegin(); }
		constexpr auto crend() const noexcept -> const_reverse_iterator { return m_data.crend(); }
		constexpr auto end() noexcept -> iterator { return m_data.end(); }
		constexpr auto end() const noexcept -> const_iterator { return m_data.end(); }
		constexpr auto rbegin() noexcept -> reverse_iterator { return m_data.rbegin(); }
		constexpr auto rbegin() const noexcept -> const_reverse_iterator { return m_data.rbegin(); }
		constexpr auto rend() noexcept -> reverse_iterator { return m_data.rend(); }
		constexpr auto rend() const noexcept -> const_reverse_iterator { return m_data.rend(); }
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr self_type & swap(self_type & other) noexcept
		{
			return m_data.swap(other.m_data), (*this);
		}

		static constexpr self_type fill(value_type value) noexcept
		{
			self_type temp{};
			for (value_type & e : temp) {
				e = value;
			}
			return temp;
		}

		static constexpr self_type one() noexcept
		{
			return fill(1);
		}

		static constexpr self_type identity() noexcept
		{
			self_type temp{};
			for (size_t i = 0; i < size(); ++i) {
				temp[i] = static_cast<value_type>(i % width() == i / height());
			}
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U, size_t W, size_t H
		> constexpr operator Matrix<U, W, H>() const noexcept
		{
			using Other = Matrix<U, W, H>;

			// same type, nothing to do
			if constexpr (std::is_same_v<Other, self_type>)
			{
				return (*this);
			}
			else
			{
				Other temp;
				if constexpr ((W == _Width) && (H == _Height))
				{
					// same dimensions
					for (size_t i = 0; i < (W * H); ++i)
					{
						temp[i] = static_cast<U>(m_data[i]);
					}
				}
				else
				{
					// different dimensions
					for (size_t i = 0; i < (W * H); ++i)
					{
						if (size_t const x{ i % W }, y{ i / W }; (x < _Width && y < _Height))
						{
							temp[i] = static_cast<U>(m_data[y * _Width + x]);
						}
					}
				}
				return temp;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// define additional code
#ifdef ISM_MATRIX_CLASS_EXTRA
		ISM_MATRIX_CLASS_EXTRA
#endif // ISM_MATRIX_CLASS_EXTRA

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// MATRIX OPERATORS
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Tx, class Ty, size_t W, size_t H
	> constexpr bool operator==(Matrix<Tx, W, H> const & lhs, Matrix<Ty, W, H> const & rhs) noexcept
	{
		return (lhs.m_data == rhs.m_data);
	}

	template <class Tx, class Ty, size_t W, size_t H
	> constexpr bool operator!=(Matrix<Tx, W, H> const & lhs, Matrix<Ty, W, H> const & rhs) noexcept
	{
		return (lhs.m_data != rhs.m_data);
	}

	template <class Tx, class Ty, size_t W, size_t H
	> constexpr bool operator<(Matrix<Tx, W, H> const & lhs, Matrix<Ty, W, H> const & rhs) noexcept
	{
		return (lhs.m_data < rhs.m_data);
	}

	template <class Tx, class Ty, size_t W, size_t H
	> constexpr bool operator<=(Matrix<Tx, W, H> const & lhs, Matrix<Ty, W, H> const & rhs) noexcept
	{
		return (lhs.m_data <= rhs.m_data);
	}

	template <class Tx, class Ty, size_t W, size_t H
	> constexpr bool operator>(Matrix<Tx, W, H> const & lhs, Matrix<Ty, W, H> const & rhs) noexcept
	{
		return (lhs.m_data > rhs.m_data);
	}

	template <class Tx, class Ty, size_t W, size_t H
	> constexpr bool operator>=(Matrix<Tx, W, H> const & lhs, Matrix<Ty, W, H> const & rhs) noexcept
	{
		return (lhs.m_data >= rhs.m_data);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Tx, class Ty, size_t W, size_t H, std::enable_if_t<mpl::is_number_v<Ty>, int> = 0
	> constexpr auto & operator+=(Matrix<Tx, W, H> & lhs, Ty const rhs) noexcept
	{
		for (auto & e : lhs) {
			e += static_cast<Tx>(rhs);
		}
		return lhs;
	}

	template <class Tx, class Ty, size_t W, size_t H, std::enable_if_t<mpl::is_number_v<Ty>, int> = 0
	> constexpr auto & operator-=(Matrix<Tx, W, H> & lhs, Ty const rhs) noexcept
	{
		for (auto & e : lhs) {
			e -= static_cast<Tx>(rhs);
		}
		return lhs;
	}

	template <class Tx, class Ty, size_t W, size_t H, std::enable_if_t<mpl::is_number_v<Ty>, int> = 0
	> constexpr auto & operator*=(Matrix<Tx, W, H> & lhs, Ty const rhs) noexcept
	{
		for (auto & e : lhs) {
			e *= static_cast<Tx>(rhs);
		}
		return lhs;
	}

	template <class Tx, class Ty, size_t W, size_t H, std::enable_if_t<mpl::is_number_v<Ty>, int> = 0
	> constexpr auto & operator/=(Matrix<Tx, W, H> & lhs, Ty const rhs) noexcept
	{
		for (auto & e : lhs) {
			e /= static_cast<Tx>(rhs);
		}
		return lhs;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Tx, class Ty, size_t W, size_t H, std::enable_if_t<mpl::is_number_v<Ty>, int> = 0
	> constexpr auto operator+(Matrix<Tx, W, H> const & lhs, Ty const rhs) noexcept
	{
		auto temp{ lhs };
		return temp += static_cast<Tx>(rhs);
	}

	template <class Tx, class Ty, size_t W, size_t H, std::enable_if_t<mpl::is_number_v<Ty>, int> = 0
	> constexpr auto operator-(Matrix<Tx, W, H> const & lhs, Ty const rhs) noexcept
	{
		auto temp{ lhs };
		return temp -= static_cast<Tx>(rhs);
	}

	template <class Tx, class Ty, size_t W, size_t H, std::enable_if_t<mpl::is_number_v<Ty>, int> = 0
	> constexpr auto operator*(Matrix<Tx, W, H> const & lhs, Ty const rhs) noexcept
	{
		auto temp{ lhs };
		return temp *= static_cast<Tx>(rhs);
	}

	template <class Tx, class Ty, size_t W, size_t H, std::enable_if_t<mpl::is_number_v<Ty>, int> = 0
	> constexpr auto operator/(Matrix<Tx, W, H> const & lhs, Ty const rhs) noexcept
	{
		auto temp{ lhs };
		return temp /= static_cast<Tx>(rhs);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, size_t W, size_t H
	> constexpr auto operator-(Matrix<T, W, H> const & lhs) noexcept
	{
		return (lhs * static_cast<T>(-1));
	}

	template <class T, size_t W, size_t H
	> constexpr auto operator+(Matrix<T, W, H> const & lhs) noexcept
	{
		return -(-(lhs));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// VECTOR OPERATORS
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Tx, class Ty, size_t N
	> constexpr auto & operator+=(TMatrixNx1<Tx, N> & lhs, TMatrixNx1<Ty, N> const & rhs) noexcept
	{
		for (size_t i = 0; i < N; ++i) {
			lhs[i] += static_cast<Tx>(rhs[i]);
		}
		return lhs;
	}

	template <class Tx, class Ty, size_t N
	> constexpr auto & operator-=(TMatrixNx1<Tx, N> & lhs, TMatrixNx1<Ty, N> const & rhs) noexcept
	{
		for (size_t i = 0; i < N; ++i) {
			lhs[i] -= static_cast<Tx>(rhs[i]);
		}
		return lhs;
	}

	template <class Tx, class Ty, size_t N
	> constexpr auto & operator*=(TMatrixNx1<Tx, N> & lhs, TMatrixNx1<Ty, N> const & rhs) noexcept
	{
		for (size_t i = 0; i < N; ++i) {
			lhs[i] *= static_cast<Tx>(rhs[i]);
		}
		return lhs;
	}

	template <class Tx, class Ty, size_t N
	> constexpr auto & operator/=(TMatrixNx1<Tx, N> & lhs, TMatrixNx1<Ty, N> const & rhs) noexcept
	{
		for (size_t i = 0; i < N; ++i) {
			lhs[i] /= static_cast<Tx>(rhs[i]);
		}
		return lhs;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Tx, class Ty, size_t N
	> constexpr auto operator+(TMatrixNx1<Tx, N> const & lhs, TMatrixNx1<Ty, N> const & rhs) noexcept
	{
		auto temp{ lhs };
		return temp += rhs;
	}

	template <class Tx, class Ty, size_t N
	> constexpr auto operator-(TMatrixNx1<Tx, N> const & lhs, TMatrixNx1<Ty, N> const & rhs) noexcept
	{
		auto temp{ lhs };
		return temp -= rhs;
	}

	template <class Tx, class Ty, size_t N
	> constexpr auto operator*(TMatrixNx1<Tx, N> const & lhs, TMatrixNx1<Ty, N> const & rhs) noexcept
	{
		auto temp{ lhs };
		return temp *= rhs;
	}

	template <class Tx, class Ty, size_t N
	> constexpr auto operator/(TMatrixNx1<Tx, N> const & lhs, TMatrixNx1<Ty, N> const & rhs) noexcept
	{
		auto temp{ lhs };
		return temp /= rhs;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// TUPLE INTERFACE
namespace std
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <size_t I, class T, size_t W, size_t H
	> constexpr T & get(ism::Matrix<T, W, H> & value) noexcept
	{
		static_assert(I < W * H, "ism::Matrix index out of bounds");
		return value.at(I);
	}

	template <size_t I, class T, size_t W, size_t H
	> constexpr T const & get(ism::Matrix<T, W, H> const & value) noexcept
	{
		static_assert(I < W * H, "ism::Matrix index out of bounds");
		return value.at(I);
	}

	template <size_t I, class T, size_t W, size_t H
	> constexpr T && get(ism::Matrix<T, W, H> && value) noexcept
	{
		static_assert(I < W * H, "ism::Matrix index out of bounds");
		return std::move(value.at(I));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <size_t X, size_t Y, class T, size_t W, size_t H
	> constexpr T & get(ism::Matrix<T, W, H> & value) noexcept
	{
		static_assert(X * Y < W * H, "ism::Matrix index out of bounds");
		return value.at(X, Y);
	}

	template <size_t X, size_t Y, class T, size_t W, size_t H
	> constexpr T const & get(ism::Matrix<T, W, H> const & value) noexcept
	{
		static_assert(X * Y < W * H, "ism::Matrix index out of bounds");
		return value.at(X, Y);
	}

	template <size_t X, size_t Y, class T, size_t W, size_t H
	> constexpr T && get(ism::Matrix<T, W, H> && value) noexcept
	{
		static_assert(X * Y < W * H, "ism::Matrix index out of bounds");
		return std::move(value.at(X, Y));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_MATRIX_HPP_
