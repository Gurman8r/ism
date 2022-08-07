#ifndef _ISM_TRANSFORM_HPP_
#define _ISM_TRANSFORM_HPP_

#include <core/math/quat.hpp>

namespace ism::util
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline Mat4 perspective(f32 fov, f32 aspect, f32 znear, f32 zfar) noexcept
	{
		return bit_cast<Mat4>(glm::perspective(fov, aspect, znear, zfar));
	}

	inline Mat4 orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 znear, f32 zfar) noexcept
	{
		return bit_cast<Mat4>(glm::ortho(left, right, bottom, top, znear, zfar));
	}

	inline Mat4 translate(Mat4 const & m, Vec3 const & v)
	{
		return bit_cast<Mat4>(glm::translate((glm::mat4 const &)m, (glm::vec3 const &)v));
	}

	inline Mat4 rotate(Mat4 const & m, f32 const angle, Vec3 const & axis) noexcept
	{
		return bit_cast<Mat4>(glm::rotate((glm::mat4 const &)m, angle, (glm::vec3 const &)axis));
	}

	inline Mat4 scale(Mat4 const & m, Vec3 const & v) noexcept
	{
		return bit_cast<Mat4>(glm::scale((glm::mat4 const &)m, (glm::vec3 const &)v));
	}

	inline Mat4 look_at(Vec3 const & eye, Vec3 const & target, Vec3 const & up = { 0, 1, 0 }) noexcept
	{
		return bit_cast<Mat4>(glm::lookAt((glm::vec3 const &)eye, (glm::vec3 const &)target, (glm::vec3 const &)up));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Transform
	{
	public:
		static constexpr Vec3 world_up{ 0, 1, 0 };

	private:
		mutable Mat4 m_matrix{ Mat4::identity() }; // keep this as the first element for conversion reasons

		mutable Mat4 m_translation{}, m_orientation{}, m_scaling{};

		mutable bool m_position_changed : 1, m_rotation_changed : 1, m_scale_changed : 1;

		Vec3 m_position{}; Quat m_rotation{}; Vec3 m_scale{};

	public:
		Transform(Vec3 const & position = {}, Quat const & rotation = {}, Vec3 const & scale = { 1, 1, 1 }) noexcept
			: m_position{ position }, m_position_changed{ true }
			, m_rotation{ rotation }, m_rotation_changed{ true }
			, m_scale{ scale }, m_scale_changed{ true }
		{
		}

		Transform(Transform const & value) noexcept
			: m_position{ value.m_position }, m_position_changed{ true }
			, m_rotation{ value.m_rotation }, m_rotation_changed{ true }
			, m_scale{ value.m_scale }, m_scale_changed{ true }
		{
		}

	public:
		/* TRANSLATION */

		NODISCARD Vec3 const & get_position() const noexcept { return m_position; }

		Transform & set_position(Vec3 const & value) noexcept
		{
			if (m_position != value) {
				m_position = value;
				m_position_changed = true;
			}
			return (*this);
		}

		Transform & set_position(f32 const x, f32 const y, f32 const z) noexcept
		{
			if (m_position[0] != x || m_position[1] != y || m_position[2] != z) {
				m_position[0] = x; m_position[1] = y; m_position[2] = z;
				m_position_changed = true;
			}
			return (*this);
		}

		Transform & translate(Vec3 const & value) noexcept
		{
			if (value != Vec3{}) {
				m_position += value;
				m_position_changed = true;
			}
			return (*this);
		}

		Transform & translate(f32 const x, f32 const y, f32 const z) noexcept
		{
			if (x != 0.f || y != 0.f || z != 0.f) {
				m_position[0] += x; m_position[1] += y; m_position[2] += z;
				m_position_changed = true;
			}
			return (*this);
		}

	public:
		/* ORIENTATION */

		NODISCARD Quat const & get_rotation() const noexcept { return m_rotation; }

		Transform & set_rotation(Quat const & value) noexcept
		{
			if (m_rotation != value) {
				m_rotation = value;
				m_rotation_changed = true;
			}
			return (*this);
		}

		Transform & set_rotation(f32 const angle, Vec3 const & axis) noexcept
		{
			return set_rotation(Quat{ angle, axis });
		}

		Transform & set_rotation(f32 const pitch, f32 const yaw, f32 const roll) noexcept
		{
			return set_rotation(Quat{ Vec3{ pitch, yaw, roll } });
		}

		Transform & set_rotation(Vec3 const & u, Vec3 const & v) noexcept
		{
			return set_rotation(Quat{ u, v });
		}

		Transform & set_rotation(Vec3 const & euler_angles) noexcept
		{
			return set_rotation(Quat{ euler_angles });
		}

		Transform & set_rotation(Mat3 const & m9) noexcept
		{
			return set_rotation(Quat{ m9 });
		}

		Transform & set_rotation(Mat4 const & m16) noexcept
		{
			return set_rotation(Quat{ m16 });
		}

		Transform & rotate(Quat const & q) noexcept
		{
			m_rotation *= q;
			m_rotation_changed = true;
			return (*this);
		}

		Transform & rotate(f32 const angle, Vec3 const & axis) noexcept
		{
			m_rotation.rotate(angle, axis);
			m_rotation_changed = true;
			return (*this);
		}

		Transform & rotate(f32 const pitch, f32 const yaw, f32 const roll) noexcept
		{
			m_rotation.rotate(pitch, yaw, roll);
			m_rotation_changed = true;
			return (*this);
		}

		Transform & rotate(Mat3 const & m9) noexcept
		{
			return rotate(Quat{ m9 });
		}

		Transform & rotate(Mat4 const & m16) noexcept
		{
			return rotate(Quat{ m16 });
		}

		Transform & look_at(Vec3 const & target, Vec3 const & up = world_up) noexcept
		{
			m_rotation.look_at(util::normalize(m_position - target), up);
			m_rotation_changed = true;
			return (*this);
		}

	public:
		/* SCALING */

		NODISCARD Vec3 const & get_scale() const noexcept { return m_scale; }

		Transform & set_scale(Vec3 const & value) noexcept
		{
			if (m_scale != value) {
				m_scale = value;
				m_scale_changed = true;
			}
			return (*this);
		}

		Transform & set_scale(f32 const x, f32 const y, f32 const z) noexcept
		{
			if (m_scale[0] != x || m_scale[1] != y || m_scale[2] != z) {
				m_scale[0] = x; m_scale[1] = y; m_scale[2] = z;
				m_scale_changed = true;
			}
			return (*this);
		}

		Transform & scale(Vec3 const & value) noexcept
		{
			if (value != Vec3{}) {
				m_scale += value;
				m_scale_changed = true;
			}
			return (*this);
		}

		Transform & scale(f32 const x, f32 const y, f32 const z) noexcept
		{
			if (x != 0.f || y != 0.f || z != 0.f) {
				m_scale[0] += x; m_scale[1] += y; m_scale[2] += z;
				m_scale_changed = true;
			}
			return (*this);
		}

	public:
		/* MATRIX */

		NODISCARD Mat4 const & get_translation() const noexcept
		{
			if (m_position_changed) {
				m_translation = util::translate(Mat4::identity(), m_position);
				m_position_changed = false;
			}
			return m_translation;
		}

		NODISCARD Mat4 const & get_orientation() const noexcept
		{
			if (m_rotation_changed) {
				m_orientation = m_rotation;
				m_rotation_changed = false;
			}
			return m_orientation;
		}

		NODISCARD Mat4 const & get_scaling() const noexcept
		{
			if (m_scale_changed) {
				m_scaling = util::scale(Mat4::identity(), m_scale);
				m_scale_changed = false;
			}
			return m_scaling;
		}

		NODISCARD Mat4 const & get_matrix() const noexcept
		{
			if (m_position_changed || m_rotation_changed || m_scale_changed) {
				m_matrix = get_scaling() * get_orientation() * get_translation();
			}
			return m_matrix;
		}

		NODISCARD operator Mat4 const & () const noexcept
		{
			return get_matrix();
		}

		NODISCARD operator f32 const * () const noexcept
		{
			return get_matrix();
		}

		NODISCARD Vec3 forward() const noexcept
		{
			return util::normalize(Vec3{ m_matrix.at(2, 0), m_matrix.at(2, 1), m_matrix.at(2, 2) });
		}

		NODISCARD Vec3 right() const noexcept
		{
			return util::normalize(util::cross(forward(), world_up));
		}

		NODISCARD Vec3 up() const noexcept
		{
			return util::normalize(util::cross(right(), forward()));
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_TRANSFORM_HPP_
