#ifndef _ISM_TRANSFORM_HPP_
#define _ISM_TRANSFORM_HPP_

#include <core/math/maths.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Transform
	{
		mutable Mat4 m_matrix{ Mat4::identity() };
		mutable bool m_changed{};

		Vec3 m_position{}, m_rotation{}, m_scale{};

	public:
		explicit Transform(
			f32 m00, f32 m01, f32 m02, f32 m03,
			f32 m10, f32 m11, f32 m12, f32 m13,
			f32 m20, f32 m21, f32 m22, f32 m23,
			f32 m30, f32 m31, f32 m32, f32 m33
		) noexcept : m_matrix{
			m00, m01, m02, m03,
			m10, m11, m12, m13,
			m20, m21, m22, m23,
			m30, m31, m32, m33
		} {}

		explicit Transform(
			f32 a00, f32 a01, f32 a02,
			f32 a10, f32 a11, f32 a12,
			f32 a20, f32 a21, f32 a22
		) noexcept : m_matrix{
			a00, a01, 0.f, a02,
			a10, a11, 0.f, a12,
			0.f, 0.f, 1.f, 0.f,
			a20, a21, 0.f, a22
		} {}

		Transform() noexcept
			: m_changed	{}
			, m_matrix	{ Mat4::identity() }
			, m_position{}
			, m_rotation{}
			, m_scale	{}
		{}

		Transform(Transform const & value) noexcept
			: m_changed	{}
			, m_matrix	{ Mat4::identity() }
			, m_position{}
			, m_rotation{}
			, m_scale	{}
		{}

		Transform(Vec3 const & position, Vec3 const & rotation, Vec3 const & scale) noexcept
			: m_changed	{ true }
			, m_matrix	{ Mat4::identity() }
			, m_position{ position }
			, m_rotation{ rotation }
			, m_scale	{ scale }
		{}

		Transform & operator=(Transform const & value) noexcept {
			Transform temp{ value };
			return swap(temp);
		}

		Transform & swap(Transform & value) noexcept {
			if (this != std::addressof(value)) {
				m_changed = value.m_changed = true;
				std::swap(m_position, value.m_position);
				std::swap(m_rotation, value.m_rotation);
				std::swap(m_scale, value.m_scale);
			}
			return (*this);
		}

	public:
		Vec3 get_position() const noexcept { return m_position; }
		void set_position(Vec3 const & value) noexcept {
			if (m_position != value) {
				m_position = value;
				m_changed = true;
			}
		}

		Vec3 get_rotation() const noexcept { return m_rotation; }
		void set_rotation(Vec3 const & value) noexcept {
			if (m_rotation != value) {
				m_rotation = value;
				m_changed = true;
			}
		}

		Vec3 get_scale() const noexcept { return m_scale; }
		void set_scale(Vec3 const & value) noexcept {
			if (m_scale != value) {
				m_scale = value;
				m_changed = true;
			}
		}

	public:
		Mat4 const & peek_matrix() const { return m_matrix; }

		Mat4 const & get_matrix() const
		{
			if (m_changed) {
				m_matrix = util::bit_cast<Mat4>(
					glm::translate(glm::mat4(1.f), (glm::vec3 const &)m_position) *
					glm::rotate(glm::mat4(1.f), 1.f, (glm::vec3 const &)m_rotation) *
					glm::scale(glm::mat4(1.f), (glm::vec3 const &)m_scale));
				m_changed = false;
			}
			return m_matrix;
		}

		operator Mat4 const & () const noexcept { return get_matrix(); }
		operator f32 const * () const noexcept { return get_matrix(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_TRANSFORM_HPP_
