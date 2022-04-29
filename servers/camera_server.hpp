#ifndef _ISM_CAMERA_SERVER_HPP_
#define _ISM_CAMERA_SERVER_HPP_

#include <core/object/detail/class.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// camera
namespace ism
{
	class ISM_API Camera : public Object
	{
		OBJECT_COMMON(Camera, Object);

		Vec2		m_resolution; // 

		Mat4		m_proj; // 
		bool		m_is_ortho; // 
		float_t		m_fov[2]; // 
		Vec2		m_clip[2]; // 

		Mat4		m_view; // 
		Vec3		m_eye; // 
		Vec3		m_target; // 
		Vec3		m_up; // 

		float_t		m_yaw; // 
		float_t		m_pitch; // 
		float_t		m_roll; // 
		float_t		m_zoom; // 

	public:
		static void frustum(float_t left, float_t right, float_t bottom, float_t top, float_t znear, float_t zfar, float_t * m16)
		{
			float_t temp, temp2, temp3, temp4;
			temp = 2.0f * znear;
			temp2 = right - left;
			temp3 = top - bottom;
			temp4 = zfar - znear;
			m16[0] = temp / temp2;
			m16[1] = 0.0;
			m16[2] = 0.0;
			m16[3] = 0.0;
			m16[4] = 0.0;
			m16[5] = temp / temp3;
			m16[6] = 0.0;
			m16[7] = 0.0;
			m16[8] = (right + left) / temp2;
			m16[9] = (top + bottom) / temp3;
			m16[10] = (-zfar - znear) / temp4;
			m16[11] = -1.0f;
			m16[12] = 0.0;
			m16[13] = 0.0;
			m16[14] = (-temp * zfar) / temp4;
			m16[15] = 0.0;
		}

		static void perspective(float_t fov_in_degrees, float_t aspect_ratio, float_t znear, float_t zfar, float_t * m16)
		{
			float_t ymax, xmax;
			ymax = znear * std::tanf(fov_in_degrees * 3.141592f / 180.0f);
			xmax = ymax * aspect_ratio;
			frustum(-xmax, xmax, -ymax, ymax, znear, zfar, m16);
		}

		static void look_at(float_t const * eye, float_t const * at, float_t const * up, float_t * m16)
		{
			static auto cross = [](float_t const * a, float_t const * b, float_t * r)
			{
				r[0] = a[1] * b[2] - a[2] * b[1];
				r[1] = a[2] * b[0] - a[0] * b[2];
				r[2] = a[0] * b[1] - a[1] * b[0];
			};

			static auto dot = [](float_t const * a, float_t const * b)
			{
				return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
			};

			static auto normalize = [](float_t const * a, float_t * r)
			{
				float_t il = 1.f / (std::sqrtf(dot(a, a)) + FLT_EPSILON);
				r[0] = a[0] * il;
				r[1] = a[1] * il;
				r[2] = a[2] * il;
			};

			float_t X[3], Y[3], Z[3], tmp[3];

			tmp[0] = eye[0] - at[0];
			tmp[1] = eye[1] - at[1];
			tmp[2] = eye[2] - at[2];

			normalize(tmp, Z);
			normalize(up, Y);

			cross(Y, Z, tmp);
			normalize(tmp, X);

			cross(Z, X, tmp);
			normalize(tmp, Y);

			m16[0] = X[0];
			m16[1] = Y[0];
			m16[2] = Z[0];
			m16[3] = 0.0f;
			m16[4] = X[1];
			m16[5] = Y[1];
			m16[6] = Z[1];
			m16[7] = 0.0f;
			m16[8] = X[2];
			m16[9] = Y[2];
			m16[10] = Z[2];
			m16[11] = 0.0f;
			m16[12] = -dot(X, eye);
			m16[13] = -dot(Y, eye);
			m16[14] = -dot(Z, eye);
			m16[15] = 1.0f;
		}

		static void orthographic(float_t l, float_t r, float_t b, float_t t, float_t zn, float_t const zf, float_t * m16)
		{
			m16[0] = 2 / (r - l);
			m16[1] = 0.0f;
			m16[2] = 0.0f;
			m16[3] = 0.0f;
			m16[4] = 0.0f;
			m16[5] = 2 / (t - b);
			m16[6] = 0.0f;
			m16[7] = 0.0f;
			m16[8] = 0.0f;
			m16[9] = 0.0f;
			m16[10] = 1.0f / (zf - zn);
			m16[11] = 0.0f;
			m16[12] = (l + r) / (l - r);
			m16[13] = (t + b) / (b - t);
			m16[14] = zn / (zn - zf);
			m16[15] = 1.0f;
		}

	public:
		Camera() noexcept
			: m_resolution	{ 1280, 720 }
			, m_proj		{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 }
			, m_is_ortho	{}
			, m_clip		{ { 0.001f, 1000.f }, { 1000.f, -1000.f } }
			, m_fov			{ 27.f, 10.f }
			, m_view		{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 }
			, m_eye			{ 5, 5, 5 }
			, m_target		{ 0, 0, 0 }
			, m_up			{ 0, 1, 0 }
			, m_yaw			{ 32.f / 180.f * 3.14159f }
			, m_pitch		{ 165.f / 180.f * 3.14159f }
			, m_roll		{ 0.f }
			, m_zoom		{ 8.f }
		{
		}

		Camera(Camera const & other)
			: m_resolution	{ other.m_resolution }
			, m_proj		{ other.m_proj }
			, m_is_ortho	{ other.m_is_ortho }
			, m_fov			{}
			, m_clip		{}
			, m_view		{ other.m_view }
			, m_eye			{ other.m_eye }
			, m_target		{ other.m_target }
			, m_up			{ other.m_up }
			, m_yaw			{ other.m_yaw }
			, m_pitch		{ other.m_pitch }
			, m_roll		{ other.m_roll }
			, m_zoom		{ other.m_zoom }
		{
			std::memcpy(m_fov, other.m_fov, sizeof(m_fov));
			std::memcpy(m_clip, other.m_clip, sizeof(m_clip));
		}

		Camera(Camera && other) noexcept : Camera{} { this->swap(std::move(other)); }

		virtual ~Camera();

	public:
		Camera & operator=(Camera const & other)
		{
			Camera temp{ other };
			this->swap(temp);
			return (*this);
		}

		Camera & operator=(Camera && other) noexcept
		{
			this->swap(std::move(other));
			return (*this);
		}

		void swap(Camera & other) noexcept
		{
			if (this != std::addressof(other))
			{
				m_resolution.swap(other.m_resolution);
				m_proj.swap(other.m_proj);
				std::swap(m_is_ortho, other.m_is_ortho);
				std::swap(m_fov, other.m_fov);
				std::swap(m_clip, other.m_clip);
				m_view.swap(other.m_view);
				m_eye.swap(other.m_eye);
				m_target.swap(other.m_target);
				m_up.swap(other.m_up);
				std::swap(m_yaw, other.m_yaw);
				std::swap(m_pitch, other.m_pitch);
				std::swap(m_roll, other.m_roll);
				std::swap(m_zoom, other.m_zoom);
			}
		}

	public:
		NODISCARD auto get_projection() const noexcept -> Mat4 const & { return m_proj; }
		NODISCARD bool is_orthographic() const noexcept { return m_is_ortho; }
		NODISCARD auto get_fov() const noexcept -> float_t { return m_fov[m_is_ortho]; }
		NODISCARD auto get_clip() const noexcept -> Vec2 const & { return m_clip[m_is_ortho]; }
		NODISCARD auto get_res() const noexcept -> Vec2 const & { return m_resolution; }

		void set_orthographic(bool value) noexcept { m_is_ortho = value; }
		void set_fov(float_t value) noexcept { m_fov[m_is_ortho] = value; }
		void set_clip(Vec2 const & value) noexcept { m_clip[m_is_ortho] = value; }
		void set_res(Vec2 const & value) noexcept { m_resolution = value; }

	public:
		NODISCARD auto get_view() const noexcept -> Mat4 const & { return m_view; }
		NODISCARD auto get_eye() const noexcept -> Vec3 const & { return m_eye; }
		NODISCARD auto get_target() const noexcept -> Vec3 const & { return m_target; }
		NODISCARD auto get_up() const noexcept -> Vec3 const & { return m_up; }

		void set_eye(Vec3 const & value) noexcept { m_eye = value; }
		void set_target(Vec3 const & value) noexcept { m_target = value; }
		void set_up(Vec3 const & value) noexcept { m_up = value; }

	public:
		NODISCARD auto get_yaw() const noexcept -> float_t { return m_yaw; }
		NODISCARD auto get_pitch() const noexcept -> float_t { return m_pitch; }
		NODISCARD auto get_roll() const noexcept -> float_t { return m_roll; }
		NODISCARD auto get_zoom() const noexcept -> float_t { return m_zoom; }

		void set_yaw(float_t value) noexcept { m_yaw = value; }
		void set_pitch(float_t value) noexcept { m_pitch = value; }
		void set_roll(float_t value) noexcept { m_roll = value; }
		void set_zoom(float_t value) noexcept { m_zoom = value; }

		void yaw(float_t value) noexcept { m_yaw += value; }
		void pitch(float_t value) noexcept { m_pitch += value; }
		void roll(float_t value) noexcept { m_roll += value; }
		void zoom(float_t value) noexcept { m_zoom += value; }

	public:
		void recalculate()
		{
			ASSERT((m_resolution[0] != 0.f) && (m_resolution[1] != 0.f));

			if (!m_is_ortho)
			{
				perspective(
					m_fov[m_is_ortho],
					m_resolution[0] / m_resolution[1],
					m_clip[m_is_ortho][0],
					m_clip[m_is_ortho][1],
					m_proj);
			}
			else
			{
				float_t const ortho_height{ m_fov[m_is_ortho] * m_resolution[1] / m_resolution[0] };
				orthographic(
					-m_fov[m_is_ortho],
					m_fov[m_is_ortho],
					-ortho_height,
					ortho_height,
					m_clip[m_is_ortho][0],
					m_clip[m_is_ortho][1],
					m_proj);
			}
			
			m_target = m_eye + Vec3{
				cosf(glm::radians(m_yaw)) * cosf(glm::radians(m_pitch)),
				sinf(glm::radians(m_pitch)),
				sinf(glm::radians(m_yaw)) * cosf(glm::radians(m_pitch))
			};
			look_at(m_eye, m_target, m_up, m_view);
		}
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// camera server
namespace ism
{
	class ISM_API CameraServer : public Object
	{
		OBJECT_COMMON(CameraServer, Object);

		static CameraServer * singleton;

	public:
		explicit CameraServer();

		virtual ~CameraServer();

		NODISCARD static CameraServer * get_singleton() noexcept { return singleton; }
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ISM_CAMERA_SERVER_HPP_
