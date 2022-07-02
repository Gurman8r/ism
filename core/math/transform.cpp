#include <core/math/transform.hpp>
#include <core/math/transform_2d.hpp>

using namespace ism;

Mat4 const & Transform::get_matrix() const
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