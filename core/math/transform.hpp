#ifndef _ISM_TRANSFORM_HPP_
#define _ISM_TRANSFORM_HPP_

#include <core/math/rect.hpp>
#include <core/math/quat.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Transform
	{
		mutable Mat4 m_matrix{};
		
		mutable bool m_changed{};

		Vec3 m_position{}, m_rotation{}, m_scale{};
		
		mutable Mat4 m_translation{}, m_orientation{}, m_scaling{};

	public:
		Transform() noexcept = default;

		Transform(Transform const & value)
			: m_changed	{ value.m_changed }
			, m_matrix	{ value.m_matrix }
			, m_position{ value.m_position }
			, m_rotation{ value.m_rotation }
			, m_scale	{ value.m_scale }
		{
		}

		Transform(Transform && value) noexcept
		{
			swap(std::move(value));
		}

		Transform(Vec3 const & position, Vec3 const & rotation, Vec3 const & scale) noexcept
			: m_changed	{ true }
			, m_position{ position }
			, m_rotation{ rotation }
			, m_scale	{ scale }
		{
			UNUSED(get_matrix());
		}

		Transform & operator=(Transform const & value)
		{
			Transform temp{ value };
			return swap(temp);
		}

		Transform & operator=(Transform && value) noexcept
		{
			return swap(std::move(value));
		}

		Transform & swap(Transform & value) noexcept
		{
			if (this != std::addressof(value))
			{
				m_changed = value.m_changed = true;
				std::swap(m_position, value.m_position);
				std::swap(m_rotation, value.m_rotation);
				std::swap(m_scale, value.m_scale);
			}
			return (*this);
		}

	public:
		Mat4 const & get_translation() const noexcept
		{
			if (m_changed) {
				((glm::mat4 &)m_translation) = glm::translate(glm::mat4(1.f), (glm::vec3 const &)m_position);
			}
			return m_translation;
		}

		Vec3 const & get_position() const noexcept
		{
			return m_position;
		}

		Transform & set_position(Vec3 const & value) noexcept
		{
			if (m_position != value) {
				m_position = value;
				m_changed = true;
			}
			return (*this);
		}

		Transform & translate(f32 const x, f32 const y, f32 const z) noexcept
		{
			m_position[0] += x;
			m_position[1] += y;
			m_position[2] += z;
			m_changed = true;
			return (*this);
		}

		Transform & translate(Vec3 const & amount) noexcept
		{
			m_position[0] += amount[0];
			m_position[1] += amount[1];
			m_position[2] += amount[2];
			m_changed = true;
			return (*this);
		}

	public:
		Mat4 const & get_orientation() const noexcept
		{
			if (m_changed) {
				((glm::mat4 &)m_orientation) = glm::rotate(glm::mat4(1.f), glm::radians(0.f), (glm::vec3 const &)m_rotation);
			}
			return m_orientation;
		}

		Vec3 const & get_rotation() const noexcept
		{
			return m_rotation;
		}

		Transform & set_rotation(Vec3 const & value) noexcept
		{
			if (m_rotation != value) {
				m_rotation = value;
				m_changed = true;
			}
			return (*this);
		}

		Transform & rotate(f32 const x, f32 const y, f32 const z, bool world = true) noexcept
		{
			m_rotation[0] += x;
			m_rotation[1] += y;
			m_rotation[2] += z;
			m_changed = true;
			return (*this);
		}

		Transform & rotate(Vec3 const & amount, bool world = true) noexcept
		{
			m_rotation[0] += amount[0];
			m_rotation[1] += amount[1];
			m_rotation[2] += amount[2];
			m_changed = true;
			return (*this);
		}

		Transform & look_at(Vec3 const & target) noexcept
		{
			return (*this);
		}

	public:
		Mat4 const & get_scaling() const noexcept
		{
			if (m_changed) {
				((glm::mat4 &)m_scaling) = glm::scale(glm::mat4(1.f), (glm::vec3 const &)m_scale);
			}
			return m_scaling;
		}

		Vec3 const & get_scale() const noexcept
		{
			return m_scale;
		}

		Transform & set_scale(Vec3 const & value) noexcept
		{
			if (m_scale != value) {
				m_scale = value;
				m_changed = true;
			}
			return (*this);
		}

		Transform & scale(f32 const x, f32 const y, f32 const z) noexcept
		{
			m_scale[0] += x;
			m_scale[1] += y;
			m_scale[2] += z;
			m_changed = true;
			return (*this);
		}

		Transform & scale(Vec3 const & amount) noexcept
		{
			m_scale[0] += amount[0];
			m_scale[1] += amount[1];
			m_scale[2] += amount[2];
			m_changed = true;
			return (*this);
		}

	public:
		Mat4 const & get_matrix() const noexcept
		{
			if (m_changed) {
				m_matrix = get_translation() * get_orientation() * get_scaling();
				m_changed = false;
			}
			return m_matrix;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_TRANSFORM_HPP_
