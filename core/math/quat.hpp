#ifndef _ISM_QUAT_HPP_
#define _ISM_QUAT_HPP_

#include <core/math/matrix.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct Quat
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr Vec3
			world_right	{ 1, 0, 0 },
			world_up	{ 0, 1, 0 },
			world_front	{ 0, 0, 1 };

		using self_type			= Quat;
		using storage_type		= Vec4;
		using value_type		= storage_type::value_type;
		using size_type			= storage_type::size_type;
		using difference_type	= storage_type::difference_type;
		using pointer			= storage_type::pointer;
		using reference			= storage_type::reference;
		using const_pointer		= storage_type::const_pointer;
		using const_reference	= storage_type::const_reference;
		using iterator			= storage_type::iterator;
		using const_iterator	= storage_type::const_iterator;

		storage_type m_data{};

		Quat(self_type const &) = default;
		Quat(self_type &&) noexcept = default;
		self_type & operator=(self_type const &) = default;
		self_type & operator=(self_type &&) noexcept = default;
		NODISCARD operator pointer() noexcept { return m_data; }
		NODISCARD operator const_pointer() const noexcept { return m_data; }
		NODISCARD operator storage_type & () & noexcept { return m_data; }
		NODISCARD operator storage_type const & () const & noexcept { return m_data; }
		NODISCARD operator storage_type && () && noexcept { return std::move(m_data); }
		NODISCARD auto begin() noexcept -> iterator { return m_data.begin(); }
		NODISCARD auto begin() const noexcept -> const_iterator { return m_data.begin(); }
		NODISCARD auto end() noexcept -> iterator { return m_data.end(); }
		NODISCARD auto end() const noexcept -> const_iterator { return m_data.end(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Quat(f32 const w = 1.f, f32 const x = 0.f, f32 const y = 0.f, f32 const z = 0.f) noexcept
		{
			::new (m_data) glm::quat{ w, x, y, z };
		}

		Quat(f32 const angle, Vec3 const & axis) noexcept
		{
			::new (m_data) glm::quat{ glm::angleAxis(angle, (glm::vec3 const &)axis) };
		}

		Quat(Vec3 const & u, Vec3 const & v) noexcept
		{
			::new (m_data) glm::quat{ (glm::vec3 const &)u, (glm::vec3 const &)v };
		}

		Quat(Vec3 const & euler_angles) noexcept
		{
			::new (m_data) glm::quat{ (glm::vec3 const &)euler_angles };
		}

		Quat(Mat3 const & m9) noexcept
		{
			::new (m_data) glm::quat{ (glm::mat3 const &)m9 };
		}

		Quat(Mat4 const & m16) noexcept
		{
			::new (m_data) glm::quat{ (glm::mat4 const &)m16 };
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD operator Mat3() const noexcept { return util::bit_cast<Mat3>(glm::toMat3((glm::quat const &)*this)); }

		NODISCARD operator Mat4() const noexcept { return util::bit_cast<Mat4>(glm::toMat4((glm::quat const &)*this)); }

		NODISCARD f32 angle() const { return glm::angle((glm::quat const &)*this); }

		NODISCARD Vec3 axis() const { return util::bit_cast<Vec3>(glm::axis((glm::quat const &)*this)); }

		NODISCARD Vec3 euler() const { return { pitch(), yaw(), roll() }; }

		NODISCARD f32 pitch() const { return glm::pitch((glm::quat const &)*this); }

		NODISCARD f32 yaw() const { return glm::yaw((glm::quat const &)*this); }

		NODISCARD f32 roll() const { return glm::roll((glm::quat const &)*this); }

		NODISCARD f32 dot(Quat const & q) const noexcept { return glm::dot((glm::quat const &)*this, (glm::quat const &)q); }

		NODISCARD f32 length() const noexcept { return glm::length((glm::quat const &)*this); }

		NODISCARD Quat cross(Quat const & q) const noexcept
		{
			auto const temp{ glm::cross((glm::quat const &)*this, (glm::quat const &)q) };
			return (Quat const &)temp;
		}

		Quat & normalize() noexcept
		{
			return glm::normalize((glm::quat const &)*this), (*this);
		}

		Quat & rotate(f32 const angle, Vec3 const & axis) noexcept
		{
			return (((glm::quat &)*this) = glm::rotate((glm::quat const &)*this, angle, (glm::vec3 const &)axis)), (*this);
		}

		Quat & rotate(f32 const pitch, f32 const yaw, f32 const roll) noexcept
		{
			return rotate(pitch, { 1, 0, 0 }).rotate(yaw, { 0, 1, 0 }).rotate(roll, { 0, 0, 1 });
		}

		Quat & rotate(Vec3 const euler_angles) noexcept
		{
			return rotate(euler_angles[0], euler_angles[1], euler_angles[2]);
		}

		Quat & look_at(Vec3 const & direction, Vec3 const & up = { 0, 1, 0 }) noexcept
		{
			return (((glm::quat &)*this) = glm::quatLookAt((glm::vec3 const &)direction, (glm::vec3 const &)up)), (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline Quat & operator+=(Quat & a, Quat const & b) noexcept { return (((glm::quat &)a) += ((glm::quat const &)b)), a; }

	inline Quat & operator-=(Quat & a, Quat const & b) noexcept { return (((glm::quat &)a) -= ((glm::quat const &)b)), a; }

	inline Quat & operator*=(Quat & a, Quat const & b) noexcept { return (((glm::quat &)a) *= ((glm::quat const &)b)), a; }

	inline Quat & operator*=(Quat & a, f32 const b) noexcept { return (((glm::quat &)a) *= b), a; }

	inline Quat & operator/=(Quat & a, f32 const b) noexcept { return (((glm::quat &)a) /= b), a; }

	inline Quat operator+(Quat const & a) noexcept { return a; }

	inline Quat operator-(Quat const & a) noexcept { return { -a[0], -a[1], -a[2], -a[3] }; }

	inline Quat operator+(Quat const & a, Quat const & b) noexcept { return Quat{ a } += b; }

	inline Quat operator-(Quat const & a, Quat const & b) noexcept { return Quat{ a } -= b; }

	inline Quat operator*(Quat const & a, Quat const & b) noexcept { return Quat{ a } *= b; }

	inline bool operator==(Quat const & a, Quat const & b) noexcept { return a.m_data == b.m_data; }

	inline bool operator!=(Quat const & a, Quat const & b) noexcept { return a.m_data != b.m_data; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_QUAT_HPP_
