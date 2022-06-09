#ifndef _ISM_SHADER_LANGUAGE_HPP_
#define _ISM_SHADER_LANGUAGE_HPP_

#include <core/io/resource.hpp>

#include <variant>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class UniformVariant
	{
	public:
		enum Type_ : size_t
		{
			Type_Void,
			Type_Bool,
			Type_Int, Type_Vec2i, Type_Vec3i, Type_Vec4i, Type_Mat2i, Type_Mat3i, Type_Mat4i,
			Type_Uint, Type_Vec2u, Type_Vec3u, Type_Vec4u, Type_Mat2u, Type_Mat3u, Type_Mat4u,
			Type_Float, Type_Vec2f, Type_Vec3f, Type_Vec4f, Type_Mat2f, Type_Mat3f, Type_Mat4f,
			Type_Double, Type_Vec2d, Type_Vec3d, Type_Vec4d, Type_Mat2d, Type_Mat3d, Type_Mat4d,
			Type_Texture,
			Type_MAX
		};

		using data_types = typename mpl::type_list
		<
			void_type,
			bool,
			int32_t, Vec2i, Vec3i, Vec4i, Mat2i, Mat3i, Mat4i,
			uint32_t, Vec2u, Vec3u, Vec4u, Mat2u, Mat3u, Mat4u,
			float_t, Vec2f, Vec3f, Vec4f, Mat2f, Mat3f, Mat4f,
			double_t, Vec2d, Vec3d, Vec4d, Mat2d, Mat3d, Mat4d,
			RID
		>;

		using storage_type = typename mpl::rename<std::variant, data_types>;

		template <class> static constexpr Type_ type_v{ Type_MAX };
		template <> static constexpr Type_ type_v<void_type>{ Type_Void };
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
		template <> static constexpr Type_ type_v<RID>{ Type_Texture };

	public:
		UniformVariant() noexcept : m_type{ Type_Void }, m_data{ void_type{} } {}

		UniformVariant(nullptr_t) noexcept : m_type{ Type_MAX }, m_data{} {}

		UniformVariant(UniformVariant const & other) : m_type{}, m_data{} { copy(other); }

		UniformVariant(UniformVariant && other) noexcept : m_type{ Type_MAX }, m_data{} { swap(other); }

		template <class T, std::enable_if_t<mpl::contains_v<T, data_types>, int> = 0
		> UniformVariant(T const & v) noexcept : m_type{ type_v<T> }, m_data{ v } {}

		UniformVariant & operator=(UniformVariant const & other) { return copy(other); }

		UniformVariant & operator=(UniformVariant && other) noexcept { return swap(other); }

		UniformVariant & operator=(nullptr_t) noexcept {
			m_type = Type_MAX;
			storage_type{}.swap(m_data);
			return (*this);
		}

		template <class T, std::enable_if_t<mpl::contains_v<T, data_types>, int> = 0
		> UniformVariant & operator=(T const & v) noexcept {
			m_type = type_v<T>;
			m_data = v;
			return (*this);
		}

		UniformVariant & copy(UniformVariant const & other) noexcept {
			if (this != std::addressof(other)) {
				m_type = other.m_type;
				m_data = other.m_data;
			}
			return (*this);
		}

		UniformVariant & swap(UniformVariant & other) noexcept {
			if (this != std::addressof(other)) {
				std::swap(m_type, other.m_type);
				m_data.swap(other.m_data);
			}
			return (*this);
		}

	public:
		NODISCARD Type_ type() const noexcept { return m_type; }
		
		NODISCARD bool holds(Type_ i) const noexcept { return i == m_type; }
		
		NODISCARD bool is_valid() const { return m_type < Type_MAX; }
		
		NODISCARD operator bool() const { return is_valid(); }

		template <Type_ I
		> NODISCARD auto get() noexcept -> mpl::nth<I, data_types> &
		{
			static_assert(I < Type_MAX);
			ASSERT(holds(I));
			return std::get<mpl::nth<I, data_types>>(m_data);
		}

		template <Type_ I
		> NODISCARD auto get() const noexcept -> mpl::nth<I, data_types> const &
		{
			static_assert(I < Type_MAX);
			ASSERT(holds(I));
			return std::get<mpl::nth<I, data_types>>(m_data);
		}

		NODISCARD void const * data() const noexcept
		{
			switch (m_type) {
			case Type_Bool: return &get<Type_Bool>();
			case Type_Int: return &get<Type_Int>();
			case Type_Vec2i: return &get<Type_Vec2i>();
			case Type_Vec3i: return &get<Type_Vec3i>();
			case Type_Vec4i: return &get<Type_Vec4i>();
			case Type_Mat2i: return &get<Type_Mat2i>();
			case Type_Mat3i: return &get<Type_Mat3i>();
			case Type_Mat4i: return &get<Type_Mat4i>();
			case Type_Uint: return &get<Type_Uint>();
			case Type_Vec2u: return &get<Type_Vec2u>();
			case Type_Vec3u: return &get<Type_Vec3u>();
			case Type_Vec4u: return &get<Type_Vec4u>();
			case Type_Mat2u: return &get<Type_Mat2u>();
			case Type_Mat3u: return &get<Type_Mat3u>();
			case Type_Mat4u: return &get<Type_Mat4u>();
			case Type_Float: return &get<Type_Float>();
			case Type_Vec2f: return &get<Type_Vec2f>();
			case Type_Vec3f: return &get<Type_Vec3f>();
			case Type_Vec4f: return &get<Type_Vec4f>();
			case Type_Mat2f: return &get<Type_Mat2f>();
			case Type_Mat3f: return &get<Type_Mat3f>();
			case Type_Mat4f: return &get<Type_Mat4f>();
			case Type_Double: return &get<Type_Double>();
			case Type_Vec2d: return &get<Type_Vec2d>();
			case Type_Vec3d: return &get<Type_Vec3d>();
			case Type_Vec4d: return &get<Type_Vec4d>();
			case Type_Mat2d: return &get<Type_Mat2d>();
			case Type_Mat3d: return &get<Type_Mat3d>();
			case Type_Mat4d: return &get<Type_Mat4d>();
			case Type_Texture: return &get<Type_Texture>();
			}
			return nullptr;
		}

		NODISCARD size_t size() const noexcept
		{
			switch (m_type) {
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
			case Type_Texture: return sizeof(RID);
			}
			return 0;
		}

	private:
		Type_ m_type;
		storage_type m_data;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API ShaderLanguage
	{
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
			Token_Sampler, Token_SamplerBuffer,

			Token_MAX
		};

		virtual void load_tokens(Vector<String> & v) const = 0;

		NODISCARD String const & get_type_name(Token_ const i) const noexcept {
			static Vector<String> v{};
			static SCOPE_ENTER(&) { load_tokens(v); };
			ASSERT(i < Token_MAX);
			ASSERT(i < v.size());
			return v[i];
		}
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
