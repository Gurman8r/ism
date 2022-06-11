#ifndef _ISM_SHADER_LANGUAGE_HPP_
#define _ISM_SHADER_LANGUAGE_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Variant final
	{
	public:
		enum Type_ : size_t
		{
			Type_Bool,
			Type_Int, Type_Vec2i, Type_Vec3i, Type_Vec4i, Type_Mat2i, Type_Mat3i, Type_Mat4i,
			Type_Uint, Type_Vec2u, Type_Vec3u, Type_Vec4u, Type_Mat2u, Type_Mat3u, Type_Mat4u,
			Type_Float, Type_Vec2f, Type_Vec3f, Type_Vec4f, Type_Mat2f, Type_Mat3f, Type_Mat4f,
			Type_Double, Type_Vec2d, Type_Vec3d, Type_Vec4d, Type_Mat2d, Type_Mat3d, Type_Mat4d,
			Type_RID,
			Type_MAX
		};

		using data_types = typename mpl::type_list
		<
			bool,
			int32_t, Vec2i, Vec3i, Vec4i, Mat2i, Mat3i, Mat4i,
			uint32_t, Vec2u, Vec3u, Vec4u, Mat2u, Mat3u, Mat4u,
			float_t, Vec2f, Vec3f, Vec4f, Mat2f, Mat3f, Mat4f,
			double_t, Vec2d, Vec3d, Vec4d, Mat2d, Mat3d, Mat4d,
			RID
		>;

		using storage_type = mpl::rename<std::variant, data_types>;

		template <class T> static constexpr bool is_valid_type_v{ mpl::contains_v<T, data_types> };

		template <Type_ T> using type_t = mpl::nth<T, data_types>;

		template <class> static constexpr Type_ type_v{ Type_MAX };
		template <> static constexpr Type_ type_v<bool>{ Type_Bool };
		template <> static constexpr Type_ type_v<int32_t>{ Type_Int };
		template <> static constexpr Type_ type_v<Vec2i>{ Type_Vec2i };
		template <> static constexpr Type_ type_v<Vec3i>{ Type_Vec3i };
		template <> static constexpr Type_ type_v<Vec4i>{ Type_Vec4i };
		template <> static constexpr Type_ type_v<Mat2i>{ Type_Mat2i };
		template <> static constexpr Type_ type_v<Mat3i>{ Type_Mat3i };
		template <> static constexpr Type_ type_v<Mat4i>{ Type_Mat4i };
		template <> static constexpr Type_ type_v<uint32_t>{ Type_Uint };
		template <> static constexpr Type_ type_v<Vec2u>{ Type_Vec2u };
		template <> static constexpr Type_ type_v<Vec3u>{ Type_Vec3u };
		template <> static constexpr Type_ type_v<Vec4u>{ Type_Vec4u };
		template <> static constexpr Type_ type_v<Mat2u>{ Type_Mat2u };
		template <> static constexpr Type_ type_v<Mat3u>{ Type_Mat3u };
		template <> static constexpr Type_ type_v<Mat4u>{ Type_Mat4u };
		template <> static constexpr Type_ type_v<float_t>{ Type_Float };
		template <> static constexpr Type_ type_v<Vec2f>{ Type_Vec2f };
		template <> static constexpr Type_ type_v<Vec3f>{ Type_Vec3f };
		template <> static constexpr Type_ type_v<Vec4f>{ Type_Vec4f };
		template <> static constexpr Type_ type_v<Mat2f>{ Type_Mat2f };
		template <> static constexpr Type_ type_v<Mat3f>{ Type_Mat3f };
		template <> static constexpr Type_ type_v<Mat4f>{ Type_Mat4f };
		template <> static constexpr Type_ type_v<double_t>{ Type_Double };
		template <> static constexpr Type_ type_v<Vec2d>{ Type_Vec2d };
		template <> static constexpr Type_ type_v<Vec3d>{ Type_Vec3d };
		template <> static constexpr Type_ type_v<Vec4d>{ Type_Vec4d };
		template <> static constexpr Type_ type_v<Mat2d>{ Type_Mat2d };
		template <> static constexpr Type_ type_v<Mat3d>{ Type_Mat3d };
		template <> static constexpr Type_ type_v<Mat4d>{ Type_Mat4d };
		template <> static constexpr Type_ type_v<RID>{ Type_RID };

		NODISCARD static constexpr size_t get_size_static(Type_ const type) noexcept
		{
			switch (type) {
			case Type_Bool: return sizeof(bool);
			case Type_Int: return sizeof(int32_t);
			case Type_Vec2i: return sizeof(Vec2i);
			case Type_Vec3i: return sizeof(Vec3i);
			case Type_Vec4i: return sizeof(Vec4i);
			case Type_Mat2i: return sizeof(Mat2i);
			case Type_Mat3i: return sizeof(Mat3i);
			case Type_Mat4i: return sizeof(Mat4i);
			case Type_Uint: return sizeof(uint32_t);
			case Type_Vec2u: return sizeof(Vec2u);
			case Type_Vec3u: return sizeof(Vec3u);
			case Type_Vec4u: return sizeof(Vec4u);
			case Type_Mat2u: return sizeof(Mat2u);
			case Type_Mat3u: return sizeof(Mat3u);
			case Type_Mat4u: return sizeof(Mat4u);
			case Type_Float: return sizeof(float_t);
			case Type_Vec2f: return sizeof(Vec2f);
			case Type_Vec3f: return sizeof(Vec3f);
			case Type_Vec4f: return sizeof(Vec4f);
			case Type_Mat2f: return sizeof(Mat2f);
			case Type_Mat3f: return sizeof(Mat3f);
			case Type_Mat4f: return sizeof(Mat4f);
			case Type_Double: return sizeof(double_t);
			case Type_Vec2d: return sizeof(Vec2d);
			case Type_Vec3d: return sizeof(Vec3d);
			case Type_Vec4d: return sizeof(Vec4d);
			case Type_Mat2d: return sizeof(Mat2d);
			case Type_Mat3d: return sizeof(Mat3d);
			case Type_Mat4d: return sizeof(Mat4d);
			case Type_RID: return sizeof(RID);
			}
			return 0;
		}

		NODISCARD static constexpr void const * get_data_static(Type_ const type, storage_type const & data) noexcept
		{
			switch (type) {
			case Type_Bool: return &std::get<bool>(data);
			case Type_Int: return &std::get<int32_t>(data);
			case Type_Vec2i: return &std::get<Vec2i>(data);
			case Type_Vec3i: return &std::get<Vec3i>(data);
			case Type_Vec4i: return &std::get<Vec4i>(data);
			case Type_Mat2i: return &std::get<Mat2i>(data);
			case Type_Mat3i: return &std::get<Mat3i>(data);
			case Type_Mat4i: return &std::get<Mat4i>(data);
			case Type_Uint: return &std::get<uint32_t>(data);
			case Type_Vec2u: return &std::get<Vec2u>(data);
			case Type_Vec3u: return &std::get<Vec3u>(data);
			case Type_Vec4u: return &std::get<Vec4u>(data);
			case Type_Mat2u: return &std::get<Mat2u>(data);
			case Type_Mat3u: return &std::get<Mat3u>(data);
			case Type_Mat4u: return &std::get<Mat4u>(data);
			case Type_Float: return &std::get<float_t>(data);
			case Type_Vec2f: return &std::get<Vec2f>(data);
			case Type_Vec3f: return &std::get<Vec3f>(data);
			case Type_Vec4f: return &std::get<Vec4f>(data);
			case Type_Mat2f: return &std::get<Mat2f>(data);
			case Type_Mat3f: return &std::get<Mat3f>(data);
			case Type_Mat4f: return &std::get<Mat4f>(data);
			case Type_Double: return &std::get<double_t>(data);
			case Type_Vec2d: return &std::get<Vec2d>(data);
			case Type_Vec3d: return &std::get<Vec3d>(data);
			case Type_Vec4d: return &std::get<Vec4d>(data);
			case Type_Mat2d: return &std::get<Mat2d>(data);
			case Type_Mat3d: return &std::get<Mat3d>(data);
			case Type_Mat4d: return &std::get<Mat4d>(data);
			case Type_RID: return &std::get<RID>(data);
			}
			return nullptr;
		}

	public:
		Variant() noexcept : m_type{ Type_MAX }, m_data{} {}

		Variant(nullptr_t) noexcept : Variant{} {}

		Variant(Variant const & v) { copy(v); }

		Variant(Variant && v) noexcept { swap(v); }

		template <class T, std::enable_if_t<is_valid_type_v<T>, int> = 0
		> Variant(T const & v) : m_type{ type_v<T> }, m_data{ v } {}

		template <class T, std::enable_if_t<is_valid_type_v<T>, int> = 0
		> Variant(T && v) noexcept : m_type{ type_v<T> }, m_data{ std::move(v) } {}

		Variant & operator=(Variant const & v) { return copy(v); }

		Variant & operator=(Variant && v) noexcept { return swap(v); }

		Variant & operator=(nullptr_t) noexcept { return invalidate(); }

		template <class T, std::enable_if_t<is_valid_type_v<T>, int> = 0
		> Variant & operator=(T const & v) { return assign(v); }

		template <class T, std::enable_if_t<is_valid_type_v<T>, int> = 0
		> Variant & operator=(T && v) noexcept { return assign(std::move(v)); }

	public:
		template <class T, std::enable_if_t<is_valid_type_v<T>, int> = 0
		> Variant & assign(T const & v) {
			m_type = type_v<T>;
			m_data = v;
			return (*this);
		}

		template <class T, std::enable_if_t<is_valid_type_v<T>, int> = 0
		> Variant & assign(T && v) noexcept {
			m_type = type_v<T>;
			m_data = std::move(v);
			return (*this);
		}

		Variant & invalidate() noexcept {
			m_type = Type_MAX;
			storage_type{}.swap(m_data);
			return (*this);
		}

		Variant & copy(Variant const & v) noexcept {
			if (this != std::addressof(v)) {
				m_type = v.m_type;
				m_data = v.m_data;
			}
			return (*this);
		}

		Variant & swap(Variant & v) noexcept {
			if (this != std::addressof(v)) {
				std::swap(m_type, v.m_type);
				m_data.swap(v.m_data);
			}
			return (*this);
		}

	public:
		NODISCARD operator bool() const noexcept { return is_valid(); }

		NODISCARD void const * data() const noexcept { return get_data_static(m_type, m_data); }

		NODISCARD size_t size() const noexcept { return get_size_static(m_type); }

		NODISCARD Type_ type() const noexcept { return m_type; }
		
		NODISCARD bool holds(Type_ const t) const noexcept { return m_type == t; }

		template <Type_ T
		> NODISCARD bool holds() const noexcept { return m_type == T; }

		template <class T, std::enable_if_t<is_valid_type_v<T>, int> = 0
		> NODISCARD bool holds() const noexcept { return m_type == type_v<T>; }
		
		NODISCARD bool is_valid() const { return m_type < Type_MAX; }

		NODISCARD bool is_bool() const { return m_type == Type_Bool; }

		NODISCARD bool is_int() const noexcept { return m_type == Type_Int; }
		
		NODISCARD bool is_uint() const noexcept { return m_type == Type_Uint; }
		
		NODISCARD bool is_float() const noexcept { return m_type == Type_Float; }
		
		NODISCARD bool is_double() const noexcept { return m_type == Type_Double; }

		NODISCARD bool is_rid() const noexcept { return m_type == Type_RID; }

		NODISCARD bool is_vector() const noexcept
		{
			return
				m_type == Type_Vec2i || m_type == Type_Vec3i || m_type == Type_Vec4i ||
				m_type == Type_Vec2u || m_type == Type_Vec3u || m_type == Type_Vec4u ||
				m_type == Type_Vec2f || m_type == Type_Vec3f || m_type == Type_Vec4f ||
				m_type == Type_Vec2d || m_type == Type_Vec3d || m_type == Type_Vec4d;
		}

		NODISCARD bool is_matrix() const noexcept
		{
			return
				m_type == Type_Mat2i || m_type == Type_Mat3i || m_type == Type_Mat4i ||
				m_type == Type_Mat2u || m_type == Type_Mat3u || m_type == Type_Mat4u ||
				m_type == Type_Mat2f || m_type == Type_Mat3f || m_type == Type_Mat4f ||
				m_type == Type_Mat2d || m_type == Type_Mat3d || m_type == Type_Mat4d;
		}

	public:
		template <class T, std::enable_if_t<is_valid_type_v<T>, int> = 0
		> NODISCARD auto get() noexcept -> T &
		{
			ASSERT(holds<T>());
			return std::get<T>(m_data);
		}

		template <class T, std::enable_if_t<is_valid_type_v<T>, int> = 0
		> NODISCARD auto get() const noexcept -> T const &
		{
			ASSERT(holds<T>());
			return std::get<T>(m_data);
		}

		template <Type_ T, std::enable_if_t<(T < Type_MAX), int> = 0
		> NODISCARD auto get() noexcept -> type_t<T> &
		{
			ASSERT(holds<T>());
			return get<type_t<T>>();
		}

		template <Type_ T, std::enable_if_t<(T < Type_MAX), int> = 0
		> NODISCARD auto get() const noexcept -> type_t<T> const &
		{
			ASSERT(holds<T>());
			return get<type_t<T>>();
		}

	public:
		NODISCARD friend bool operator==(Variant const & a, Variant const & b) noexcept
		{
			return (std::addressof(a) == std::addressof(b)) || (a.m_type == b.m_type && a.m_data == b.m_data);
		}

		NODISCARD friend bool operator!=(Variant const & a, Variant const & b) noexcept
		{
			return (std::addressof(a) != std::addressof(b)) && (a.m_type != b.m_type || a.m_data != b.m_data);
		}

	private:
		Type_ m_type{ Type_MAX };
		storage_type m_data{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API ShaderLanguage
	{
		mutable Vector<String> m_tokens;

	public:
		virtual ~ShaderLanguage() noexcept = default;

	public:
		enum Token_ : size_t
		{
			Token_Void,
			Token_Bool,
			Token_Int, Token_Vec2i, Token_Vec3i, Token_Vec4i, Token_Mat2i, Token_Mat3i, Token_Mat4i,
			Token_Uint, Token_Vec2u, Token_Vec3u, Token_Vec4u, Token_Mat2u, Token_Mat3u, Token_Mat4u,
			Token_Float, Token_Vec2f, Token_Vec3f, Token_Vec4f, Token_Mat2f, Token_Mat3f, Token_Mat4f,
			Token_Double, Token_Vec2d, Token_Vec3d, Token_Vec4d, Token_Mat2d, Token_Mat3d, Token_Mat4d,
			Token_Texture2D, Token_Texture3D, Token_TextureCube,
			Token_MAX
		};

		virtual void load_tokens(Vector<String> & v) const = 0;

		NODISCARD String const & get_token(Token_ i) const noexcept {
			ASSERT(i < Token_MAX);
			if (m_tokens.empty()) { load_tokens(m_tokens); }
			ASSERT(i < m_tokens.size());
			return m_tokens[i];
		}

	public:
		struct BaseNode
		{
		};

		struct ShaderNode : BaseNode
		{
			struct Uniform
			{
				int32_t order{ -1 };
				int32_t texture_order{ -1 };
				Variant::Type_ type{ Variant::Type_MAX };
			};
		};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API ShaderLanguageGLSL : public ShaderLanguage
	{
	public:
		virtual ~ShaderLanguageGLSL() noexcept override = default;

		virtual void load_tokens(Vector<String> & v) const override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_SHADER_LANGUAGE_HPP_
