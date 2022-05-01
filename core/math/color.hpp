#ifndef _ISM_COLOR_HPP_
#define _ISM_COLOR_HPP_

#include <core/math/vector4.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace util
	{
		template <class To, class From
		> NODISCARD constexpr TVec4<To> color_cast(const TVec4<From> & value) noexcept
		{
			return (TVec4<To>)value;
		}

		NODISCARD constexpr Vec4b color_cast(Vec4f const & value) noexcept
		{
			return (Vec4b)(value * 255.f);
		}

		NODISCARD constexpr Vec4f color_cast(Vec4b const & value) noexcept
		{
			return ((Vec4f)value) / 255.f;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	template <class _T> struct NODISCARD basic_color final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type					= typename basic_color<_T>;
		using value_type				= typename _T;
		using rgb_type					= typename TVec3<value_type>;
		using rgba_type					= typename TVec4<value_type>;
		using size_type					= typename rgba_type::size_type;
		using difference_type			= typename rgba_type::difference_type;
		using pointer					= typename rgba_type::pointer;
		using reference					= typename rgba_type::reference;
		using const_pointer				= typename rgba_type::const_pointer;
		using const_reference			= typename rgba_type::const_reference;
		using iterator					= typename rgba_type::iterator;
		using const_iterator			= typename rgba_type::const_iterator;
		using reverse_iterator			= typename rgba_type::reverse_iterator;
		using const_reverse_iterator	= typename rgba_type::const_reverse_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr basic_color(rgba_type const & value)
			: m_data{ value }
		{
		}

		constexpr basic_color(rgb_type const & rgb, value_type a)
			: self_type{ rgb[0], rgb[1], rgb[2], a }
		{
		}

		constexpr basic_color(value_type rgba)
			: m_data{ rgba, rgba, rgba, rgba }
		{
		}

		template <class R, class G, class B, class A
		> constexpr basic_color(R r, G g, B b, A a) : m_data{
			static_cast<value_type>(r),
			static_cast<value_type>(g),
			static_cast<value_type>(b),
			static_cast<value_type>(a)
		}
		{
		}

		template <class R, class G, class B
		> constexpr basic_color(R r, G g, B b) : self_type{ r, g, b, 1 }
		{
		}

		template <class U> constexpr basic_color(TVec4<U> const & value)
			: m_data{ util::color_cast(value) }
		{
		}

		template <class U> constexpr basic_color(basic_color<U> const & value)
			: m_data{ util::color_cast(value.rgba()) }
		{
		}

		constexpr basic_color()
			: m_data{ 0 }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				m_data.swap(other.m_data);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		operator rgba_type & () & noexcept { return m_data; }

		constexpr operator rgba_type const & () const & noexcept { return m_data; }

		operator rgba_type && () && noexcept { return std::move(m_data); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr operator pointer() noexcept { return m_data; }

		constexpr operator const_pointer() const noexcept { return m_data; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr auto r() const  noexcept-> value_type const & { return m_data[0]; }
		
		constexpr auto g() const  noexcept-> value_type const & { return m_data[1]; }
		
		constexpr auto b() const  noexcept-> value_type const & { return m_data[2]; }
		
		constexpr auto a() const  noexcept-> value_type const & { return m_data[3]; }
		
		constexpr auto rgb() const  noexcept-> rgb_type { return (rgb_type)m_data; }

		constexpr auto rgba() & noexcept-> rgba_type & { return m_data; }
		
		constexpr auto rgba() const & noexcept-> rgba_type const & { return m_data; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		constexpr auto begin() noexcept -> iterator { return m_data.begin(); }

		constexpr auto begin() const noexcept -> const_iterator { return m_data.begin(); }

		constexpr auto cbegin() const noexcept -> const_iterator { return m_data.cbegin(); }

		constexpr auto cend() const noexcept -> const_iterator { return m_data.cend(); }

		constexpr auto crbegin() const noexcept -> const_reverse_iterator { return m_data.crbegin(); }

		constexpr auto crend() const noexcept -> const_reverse_iterator { return m_data.crend(); }

		constexpr auto end() noexcept -> iterator { return m_data.end(); }

		constexpr auto end() const noexcept -> const_iterator { return m_data.end(); }

		constexpr auto rbegin() noexcept -> reverse_iterator { return m_data.rbegin(); }

		constexpr auto rbegin() const noexcept -> const_reverse_iterator { return m_data.rbegin(); }

		constexpr auto rend() noexcept -> reverse_iterator { return m_data.rend(); }

		constexpr auto rend() const noexcept -> const_reverse_iterator { return m_data.rend(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		rgba_type m_data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ALIAS(Color) basic_color<float_t>;
	
	ALIAS(Color32) uint32_t;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Colors final
	{
	public:
		static constexpr Color clear	{ 0.0f, 0.0f, 0.0f, 0.0f };
		static constexpr Color white	{ 1.0f, 1.0f, 1.0f, 1.0f };
		static constexpr Color gray		{ 0.5f, 0.5f, 0.5f, 1.0f };
		static constexpr Color black	{ 0.0f, 0.0f, 0.0f, 1.0f };

		static constexpr Color red		{ 1.0f, 0.0f, 0.0f, 1.0f };
		static constexpr Color green	{ 0.0f, 1.0f, 0.0f, 1.0f };
		static constexpr Color blue		{ 0.0f, 0.0f, 1.0f, 1.0f };
		static constexpr Color cyan		{ 0.0f, 1.0f, 1.0f, 1.0f };
		static constexpr Color yellow	{ 1.0f, 1.0f, 0.0f, 1.0f };
		static constexpr Color magenta	{ 1.0f, 0.0f, 1.0f, 1.0f };
		static constexpr Color violet	{ 0.5f, 0.0f, 1.0f, 1.0f };
		static constexpr Color lime		{ 0.5f, 1.0f, 0.0f, 1.0f };
		static constexpr Color orange	{ 1.0f, 0.5f, 0.0f, 1.0f };
		static constexpr Color fuchsia	{ 1.0f, 0.0f, 0.5f, 1.0f };
		static constexpr Color aqua		{ 0.0f, 1.0f, 0.5f, 1.0f };
		static constexpr Color azure	{ 0.0f, 0.5f, 1.0f, 1.0f };
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	NODISCARD inline Color rotate_hue(Color const & v, float_t degrees) noexcept
	{
		// https://stackoverflow.com/a/8510751
		float_t const c{ std::cos(deg2rad(degrees)) }, s{ std::sin(deg2rad(degrees)) };
		auto m{ Mat3::identity() };
		m.at(0, 0) = c + (1.0f - c) / 3.0f;
		m.at(0, 1) = 1.f / 3.f * (1.0f - c) - std::sqrt(1.f / 3.f) * s;
		m.at(0, 2) = 1.f / 3.f * (1.0f - c) + std::sqrt(1.f / 3.f) * s;
		m.at(1, 0) = 1.f / 3.f * (1.0f - c) + std::sqrt(1.f / 3.f) * s;
		m.at(1, 1) = c + 1.f / 3.f * (1.0f - c);
		m.at(1, 2) = 1.f / 3.f * (1.0f - c) - std::sqrt(1.f / 3.f) * s;
		m.at(2, 0) = 1.f / 3.f * (1.0f - c) - std::sqrt(1.f / 3.f) * s;
		m.at(2, 1) = 1.f / 3.f * (1.0f - c) + std::sqrt(1.f / 3.f) * s;
		m.at(2, 2) = c + 1.f / 3.f * (1.0f - c);
		return {
			v[0] * m.at(0, 0) + v[1] * m.at(0, 1) + v[2] * m.at(0, 2),
			v[0] * m.at(1, 0) + v[1] * m.at(1, 1) + v[2] * m.at(1, 2),
			v[0] * m.at(2, 0) + v[1] * m.at(2, 1) + v[2] * m.at(2, 2),
			1.f
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_COLOR_HPP_
