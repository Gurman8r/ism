#ifndef _ISM_VAR_HPP_
#define _ISM_VAR_HPP_

#include <core/io/resource.hpp>

#include <variant>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum VarType_ : size_t
	{
		VarType_bool, VarType_i32, VarType_i64, VarType_u32, VarType_u64, VarType_f32, VarType_f64,

		VarType_Vec2i, VarType_Vec2l, VarType_Vec2u, VarType_Vec2ul, VarType_Vec2f, VarType_Vec2d,

		VarType_Vec3i, VarType_Vec3l, VarType_Vec3u, VarType_Vec3ul, VarType_Vec3f, VarType_Vec3d,

		VarType_Vec4i, VarType_Vec4l, VarType_Vec4u, VarType_Vec4ul, VarType_Vec4f, VarType_Vec4d,

		VarType_Mat2i, VarType_Mat2l, VarType_Mat2u, VarType_Mat2ul, VarType_Mat2f, VarType_Mat2d,

		VarType_Mat3i, VarType_Mat3l, VarType_Mat3u, VarType_Mat3ul, VarType_Mat3f, VarType_Mat3d,

		VarType_Mat4i, VarType_Mat4l, VarType_Mat4u, VarType_Mat4ul, VarType_Mat4f, VarType_Mat4d,

		VarType_RID, VarType_Object, VarType_String,

		VarType_MAX
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Var final
	{
	public:
		using types = typename mpl::type_list
		<
			bool, i32, i64, u32, u64, f32, f64,

			Vec2i, Vec2l, Vec2u, Vec2ul, Vec2f, Vec2d,

			Vec3i, Vec3l, Vec3u, Vec3ul, Vec3f, Vec3d,

			Vec4i, Vec4l, Vec4u, Vec4ul, Vec4f, Vec4d,

			Mat2i, Mat2l, Mat2u, Mat2ul, Mat2f, Mat2d,

			Mat3i, Mat3l, Mat3u, Mat3ul, Mat3f, Mat3d,

			Mat4i, Mat4l, Mat4u, Mat4ul, Mat4f, Mat4d,

			RID, ObjectRef, String
		>;

		using storage = typename mpl::rename<std::variant, types>;

		template <class T> static constexpr bool is_valid_type_v{ mpl::contains_v<T, types> };

		template <VarType_ T> using type_t = mpl::nth<T, types>;

	private:
		VarType_ m_type{ VarType_MAX }; // VAR TYPE
		storage m_data{}; // VAR DATA

	public:
		template <class> static constexpr VarType_ type_v{ VarType_MAX };
		template <> static constexpr VarType_ type_v<bool>{ VarType_bool };
		template <> static constexpr VarType_ type_v<i32>{ VarType_i32 };
		template <> static constexpr VarType_ type_v<i64>{ VarType_i64 };
		template <> static constexpr VarType_ type_v<u32>{ VarType_u32 };
		template <> static constexpr VarType_ type_v<u64>{ VarType_u64 };
		template <> static constexpr VarType_ type_v<f32>{ VarType_f32 };
		template <> static constexpr VarType_ type_v<f64>{ VarType_f64 };

		template <> static constexpr VarType_ type_v<Vec2i>{ VarType_Vec2i };
		template <> static constexpr VarType_ type_v<Vec2l>{ VarType_Vec2l };
		template <> static constexpr VarType_ type_v<Vec2u>{ VarType_Vec2u };
		template <> static constexpr VarType_ type_v<Vec2ul>{ VarType_Vec2ul };
		template <> static constexpr VarType_ type_v<Vec2f>{ VarType_Vec2f };
		template <> static constexpr VarType_ type_v<Vec2d>{ VarType_Vec2d };

		template <> static constexpr VarType_ type_v<Vec3i>{ VarType_Vec3i };
		template <> static constexpr VarType_ type_v<Vec3l>{ VarType_Vec3l };
		template <> static constexpr VarType_ type_v<Vec3u>{ VarType_Vec3u };
		template <> static constexpr VarType_ type_v<Vec3ul>{ VarType_Vec3ul };
		template <> static constexpr VarType_ type_v<Vec3f>{ VarType_Vec3f };
		template <> static constexpr VarType_ type_v<Vec3d>{ VarType_Vec3d };

		template <> static constexpr VarType_ type_v<Vec4i>{ VarType_Vec4i };
		template <> static constexpr VarType_ type_v<Vec4l>{ VarType_Vec4l };
		template <> static constexpr VarType_ type_v<Vec4u>{ VarType_Vec4u };
		template <> static constexpr VarType_ type_v<Vec4ul>{ VarType_Vec4ul };
		template <> static constexpr VarType_ type_v<Vec4f>{ VarType_Vec4f };
		template <> static constexpr VarType_ type_v<Vec4d>{ VarType_Vec4d };

		template <> static constexpr VarType_ type_v<Mat2i>{ VarType_Mat2i };
		template <> static constexpr VarType_ type_v<Mat2l>{ VarType_Mat2l };
		template <> static constexpr VarType_ type_v<Mat2u>{ VarType_Mat2u };
		template <> static constexpr VarType_ type_v<Mat2ul>{ VarType_Mat2ul };
		template <> static constexpr VarType_ type_v<Mat2f>{ VarType_Mat2f };
		template <> static constexpr VarType_ type_v<Mat2d>{ VarType_Mat2d };

		template <> static constexpr VarType_ type_v<Mat3i>{ VarType_Mat3i };
		template <> static constexpr VarType_ type_v<Mat3l>{ VarType_Mat3l };
		template <> static constexpr VarType_ type_v<Mat3u>{ VarType_Mat3u };
		template <> static constexpr VarType_ type_v<Mat3ul>{ VarType_Mat3ul };
		template <> static constexpr VarType_ type_v<Mat3f>{ VarType_Mat3f };
		template <> static constexpr VarType_ type_v<Mat3d>{ VarType_Mat3d };

		template <> static constexpr VarType_ type_v<Mat4i>{ VarType_Mat4i };
		template <> static constexpr VarType_ type_v<Mat4l>{ VarType_Mat4l };
		template <> static constexpr VarType_ type_v<Mat4u>{ VarType_Mat4u };
		template <> static constexpr VarType_ type_v<Mat4ul>{ VarType_Mat4ul };
		template <> static constexpr VarType_ type_v<Mat4f>{ VarType_Mat4f };
		template <> static constexpr VarType_ type_v<Mat4d>{ VarType_Mat4d };

		template <> static constexpr VarType_ type_v<RID>{ VarType_RID };
		template <> static constexpr VarType_ type_v<ObjectRef>{ VarType_Object };
		template <> static constexpr VarType_ type_v<char *>{ VarType_String };
		template <> static constexpr VarType_ type_v<cstring>{ VarType_String };
		template <> static constexpr VarType_ type_v<String>{ VarType_String };

		static constexpr size_t get_size_static(VarType_ const type) noexcept
		{
			switch (type) {
			case VarType_bool: return sizeof(bool);
			case VarType_i32: return sizeof(i32);
			case VarType_i64: return sizeof(i64);
			case VarType_u32: return sizeof(u32);
			case VarType_u64: return sizeof(u64);
			case VarType_f32: return sizeof(f32);
			case VarType_f64: return sizeof(f64);

			case VarType_Vec2i: return sizeof(Vec2i);
			case VarType_Vec2l: return sizeof(Vec2l);
			case VarType_Vec2u: return sizeof(Vec2u);
			case VarType_Vec2ul: return sizeof(Vec2ul);
			case VarType_Vec2f: return sizeof(Vec2f);
			case VarType_Vec2d: return sizeof(Vec2d);

			case VarType_Vec3i: return sizeof(Vec3i);
			case VarType_Vec3l: return sizeof(Vec3l);
			case VarType_Vec3u: return sizeof(Vec3u);
			case VarType_Vec3ul: return sizeof(Vec3ul);
			case VarType_Vec3f: return sizeof(Vec3f);
			case VarType_Vec3d: return sizeof(Vec3d);

			case VarType_Vec4i: return sizeof(Vec4i);
			case VarType_Vec4l: return sizeof(Vec4l);
			case VarType_Vec4u: return sizeof(Vec4u);
			case VarType_Vec4ul: return sizeof(Vec4ul);
			case VarType_Vec4f: return sizeof(Vec4f);
			case VarType_Vec4d: return sizeof(Vec4d);

			case VarType_Mat2i: return sizeof(Mat2i);
			case VarType_Mat2l: return sizeof(Mat2l);
			case VarType_Mat2u: return sizeof(Mat2u);
			case VarType_Mat2ul: return sizeof(Mat2ul);
			case VarType_Mat2f: return sizeof(Mat2f);
			case VarType_Mat2d: return sizeof(Mat2d);

			case VarType_Mat3i: return sizeof(Mat3i);
			case VarType_Mat3l: return sizeof(Mat3l);
			case VarType_Mat3u: return sizeof(Mat3u);
			case VarType_Mat3ul: return sizeof(Mat3ul);
			case VarType_Mat3f: return sizeof(Mat3f);
			case VarType_Mat3d: return sizeof(Mat3d);

			case VarType_Mat4i: return sizeof(Mat4i);
			case VarType_Mat4l: return sizeof(Mat4l);
			case VarType_Mat4u: return sizeof(Mat4u);
			case VarType_Mat4ul: return sizeof(Mat4ul);
			case VarType_Mat4f: return sizeof(Mat4f);
			case VarType_Mat4d: return sizeof(Mat4d);

			case VarType_RID: return sizeof(RID);
			case VarType_Object: return sizeof(ObjectRef);
			case VarType_String: return sizeof(String);
			}
			return 0;
		}

		static constexpr void const * get_data_static(VarType_ const type, storage const & data) noexcept
		{
			switch (type) {
			case VarType_bool: return &std::get<bool>(data);
			case VarType_i32: return &std::get<i32>(data);
			case VarType_i64: return &std::get<i64>(data);
			case VarType_u32: return &std::get<u32>(data);
			case VarType_u64: return &std::get<u64>(data);
			case VarType_f32: return &std::get<f32>(data);
			case VarType_f64: return &std::get<f64>(data);

			case VarType_Vec2i: return &std::get<Vec2i>(data);
			case VarType_Vec2l: return &std::get<Vec2l>(data);
			case VarType_Vec2u: return &std::get<Vec2u>(data);
			case VarType_Vec2ul: return &std::get<Vec2ul>(data);
			case VarType_Vec2f: return &std::get<Vec2f>(data);
			case VarType_Vec2d: return &std::get<Vec2d>(data);

			case VarType_Vec3i: return &std::get<Vec3i>(data);
			case VarType_Vec3l: return &std::get<Vec3l>(data);
			case VarType_Vec3u: return &std::get<Vec3u>(data);
			case VarType_Vec3ul: return &std::get<Vec3ul>(data);
			case VarType_Vec3f: return &std::get<Vec3f>(data);
			case VarType_Vec3d: return &std::get<Vec3d>(data);

			case VarType_Vec4i: return &std::get<Vec4i>(data);
			case VarType_Vec4l: return &std::get<Vec4l>(data);
			case VarType_Vec4u: return &std::get<Vec4u>(data);
			case VarType_Vec4ul: return &std::get<Vec4ul>(data);
			case VarType_Vec4f: return &std::get<Vec4f>(data);
			case VarType_Vec4d: return &std::get<Vec4d>(data);

			case VarType_Mat2i: return &std::get<Mat2i>(data);
			case VarType_Mat2l: return &std::get<Mat2l>(data);
			case VarType_Mat2u: return &std::get<Mat2u>(data);
			case VarType_Mat2ul: return &std::get<Mat2ul>(data);
			case VarType_Mat2f: return &std::get<Mat2f>(data);
			case VarType_Mat2d: return &std::get<Mat2d>(data);

			case VarType_Mat3i: return &std::get<Mat3i>(data);
			case VarType_Mat3l: return &std::get<Mat3l>(data);
			case VarType_Mat3u: return &std::get<Mat3u>(data);
			case VarType_Mat3ul: return &std::get<Mat3ul>(data);
			case VarType_Mat3f: return &std::get<Mat3f>(data);
			case VarType_Mat3d: return &std::get<Mat3d>(data);

			case VarType_Mat4i: return &std::get<Mat4i>(data);
			case VarType_Mat4l: return &std::get<Mat4l>(data);
			case VarType_Mat4u: return &std::get<Mat4u>(data);
			case VarType_Mat4ul: return &std::get<Mat4ul>(data);
			case VarType_Mat4f: return &std::get<Mat4f>(data);
			case VarType_Mat4d: return &std::get<Mat4d>(data);

			case VarType_RID: return &std::get<RID>(data);
			case VarType_Object: return &std::get<ObjectRef>(data);
			case VarType_String: return &std::get<String>(data);
			}
			return nullptr;
		}

	public:
		Var() noexcept : m_type{ VarType_MAX }, m_data{} {}

		Var(nullptr_t) noexcept : Var{} {}

		Var(Var const & v) { copy(v); }

		Var(Var && v) noexcept { swap(v); }

		template <class T, std::enable_if_t<is_valid_type_v<T>, int> = 0
		> Var(T const & v) : m_type{ type_v<T> }, m_data{ v } {}

		template <class T, std::enable_if_t<is_valid_type_v<T>, int> = 0
		> Var(T && v) noexcept : m_type{ type_v<T> }, m_data{ std::move(v) } {}

		Var & operator=(Var const & v) { return copy(v); }

		Var & operator=(Var && v) noexcept { return swap(v); }

		Var & operator=(nullptr_t) noexcept { return invalidate(); }

		template <class T, std::enable_if_t<is_valid_type_v<T>, int> = 0
		> Var & operator=(T const & v) { return assign(v); }

		template <class T, std::enable_if_t<is_valid_type_v<T>, int> = 0
		> Var & operator=(T && v) noexcept { return assign(std::move(v)); }

	public:
		template <class T, std::enable_if_t<is_valid_type_v<T>, int> = 0
		> Var & assign(T const & v) {
			m_type = type_v<T>;
			m_data = v;
			return (*this);
		}

		template <class T, std::enable_if_t<is_valid_type_v<T>, int> = 0
		> Var & assign(T && v) noexcept {
			m_type = type_v<T>;
			m_data = std::move(v);
			return (*this);
		}

		Var & invalidate() noexcept {
			m_type = VarType_MAX;
			storage{}.swap(m_data);
			return (*this);
		}

		Var & copy(Var const & v) noexcept {
			if (this != std::addressof(v)) {
				m_type = v.m_type;
				m_data = v.m_data;
			}
			return (*this);
		}

		Var & swap(Var & v) noexcept {
			if (this != std::addressof(v)) {
				util::swap(m_type, v.m_type);
				m_data.swap(v.m_data);
			}
			return (*this);
		}

	public:
		NODISCARD operator bool() const noexcept { return is_valid(); }

		NODISCARD void const * data() const noexcept { return get_data_static(m_type, m_data); }

		NODISCARD size_t size() const noexcept { return get_size_static(m_type); }

		NODISCARD VarType_ type() const noexcept { return m_type; }
		
		NODISCARD bool holds(VarType_ const t) const noexcept { return m_type == t; }

		template <VarType_ T
		> NODISCARD bool holds() const noexcept { return m_type == T; }

		template <class T, std::enable_if_t<is_valid_type_v<T>, int> = 0
		> NODISCARD bool holds() const noexcept { return m_type == type_v<T>; }
		
		NODISCARD bool is_valid() const { return m_type < VarType_MAX; }

		NODISCARD bool is_bool() const { return m_type == VarType_bool; }

		NODISCARD bool is_signed() const noexcept { return m_type == VarType_i32 || m_type == VarType_i64; }
		
		NODISCARD bool is_unsigned() const noexcept { return m_type == VarType_u32 || m_type == VarType_u64; }

		NODISCARD bool is_int() const noexcept { return is_signed() || is_unsigned(); }
		
		NODISCARD bool is_float() const noexcept { return m_type == VarType_f32; }
		
		NODISCARD bool is_double() const noexcept { return m_type == VarType_f64; }

		NODISCARD bool is_vector() const noexcept { return m_type >= VarType_Vec2i && m_type <= VarType_Vec4d; }

		NODISCARD bool is_matrix() const noexcept { return m_type >= VarType_Mat2i && m_type <= VarType_Mat4d; }

		NODISCARD bool is_rid() const noexcept { return m_type == VarType_RID; }

		NODISCARD bool is_object() const noexcept { return m_type == VarType_Object; }

		NODISCARD bool is_string() const noexcept { return m_type == VarType_String; }

	public:
		template <class T, std::enable_if_t<is_valid_type_v<T>, int> = 0
		> NODISCARD auto get() noexcept -> T & {
			ASSERT(holds<T>());
			return std::get<T>(m_data);
		}

		template <class T, std::enable_if_t<is_valid_type_v<T>, int> = 0
		> NODISCARD auto get() const noexcept -> T const & {
			ASSERT(holds<T>());
			return std::get<T>(m_data);
		}

		template <VarType_ T, std::enable_if_t<(T < VarType_MAX), int> = 0
		> NODISCARD auto get() noexcept -> type_t<T> & {
			ASSERT(holds<T>());
			return get<type_t<T>>();
		}

		template <VarType_ T, std::enable_if_t<(T < VarType_MAX), int> = 0
		> NODISCARD auto get() const noexcept -> type_t<T> const & {
			ASSERT(holds<T>());
			return get<type_t<T>>();
		}

	public:
		friend bool operator==(Var const & a, Var const & b) noexcept {
			return (std::addressof(a) == std::addressof(b))
				|| (a.m_type == b.m_type && a.m_data == b.m_data);
		}

		friend bool operator!=(Var const & a, Var const & b) noexcept {
			return (std::addressof(a) != std::addressof(b))
				&& (a.m_type != b.m_type || a.m_data != b.m_data);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct TypeCaster<Var>
	{
		bool load(ObjectRef const & src, bool)
		{
			if (!src) {
				return (value = Var{}), false;
			}
			else {
				return (value = src), true;
			}
		}

		static ObjectRef cast(Var const & src, ReturnValuePolicy_, ObjectRef const &)
		{
			if (src.is_object()) {
				return src.get<ObjectRef>();
			}
			else {
				return nullptr;
			}
		}

		TYPE_CASTER_COMMON(Var, "var");
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_VAR_HPP_
