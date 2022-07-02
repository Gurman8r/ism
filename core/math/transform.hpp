#ifndef _ISM_TRANSFORM_HPP_
#define _ISM_TRANSFORM_HPP_

#include <core/math/color.hpp>
#include <core/math/rect.hpp>
#include <core/math/quat.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Transform2D;

	class ISM_API Transform
	{
		mutable Mat4 m_matrix{};
		mutable bool m_changed{};

		Vec3 m_position{}, m_rotation{}, m_scale{};

	public:
		explicit Transform(
			float_t m00, float_t m01, float_t m02, float_t m03,
			float_t m10, float_t m11, float_t m12, float_t m13,
			float_t m20, float_t m21, float_t m22, float_t m23,
			float_t m30, float_t m31, float_t m32, float_t m33
		) noexcept : m_matrix{
			m00, m01, m02, m03,
			m10, m11, m12, m13,
			m20, m21, m22, m23,
			m30, m31, m32, m33
		} {}

		explicit Transform(
			float_t a00, float_t a01, float_t a02,
			float_t a10, float_t a11, float_t a12,
			float_t a20, float_t a21, float_t a22
		) noexcept : m_matrix{
			a00, a01, 0.f, a02,
			a10, a11, 0.f, a12,
			0.f, 0.f, 1.f, 0.f,
			a20, a21, 0.f, a22
		} {}

		Transform() noexcept : m_matrix{ Mat4::identity() } {}

		Transform(Mat4 const & value) noexcept : m_matrix{ value } {}

		Transform(Transform const & value) { copy(value); }

		Transform(Transform && value) noexcept { swap(value); }

		Transform & operator=(Transform const & value) { return copy(value), (*this); }

		Transform & operator=(Transform && value) noexcept { return swap(value), (*this); }

		void copy(Transform const & value)
		{
			if (this != std::addressof(value))
			{
				m_matrix = value.m_matrix;
				m_changed = value.m_changed;
				m_position = value.m_position;
				m_rotation = value.m_rotation;
				m_scale = value.m_scale;
			}
		}

		void swap(Transform & value) noexcept
		{
			if (this != std::addressof(value))
			{
				std::swap(m_matrix, value.m_matrix);
				std::swap(m_changed, value.m_changed);
				std::swap(m_position, value.m_position);
				std::swap(m_rotation, value.m_rotation);
				std::swap(m_scale, value.m_scale);
			}
		}

	public:
		auto get_position() const noexcept -> Vec3 const & { return m_position; }
		void set_position(Vec3 const & value) noexcept { if (m_position != value) { m_position = value; m_changed = true; } }

		auto get_rotation() const noexcept -> Vec3 const & { return m_rotation; }
		void set_rotation(Vec3 const & value) noexcept { if (m_rotation != value) { m_rotation = value; m_changed = true; } }

		auto get_scale() const noexcept -> Vec3 const & { return m_scale; }
		void set_scale(Vec3 const & value) noexcept { if (m_scale != value) { m_scale = value; m_changed = true; } }

		NODISCARD Mat4 const & get_matrix() const;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_TRANSFORM_HPP_
