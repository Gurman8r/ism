#ifndef _ISM_COLOR_HPP_
#define _ISM_COLOR_HPP_

#include <core/math/matrix.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// 32-bit Color
	ALIAS(Color32) u32;

#define COLOR32_R_SHIFT	0
#define COLOR32_G_SHIFT	8
#define COLOR32_B_SHIFT	16
#define COLOR32_A_SHIFT	24
#define COLOR32_A_MASK	0xFF000000

	// Make Color32
#define COLOR32(r, g, b, a) \
		((Color32)((u8)(a)) << COLOR32_A_SHIFT) | \
		((Color32)((u8)(b)) << COLOR32_B_SHIFT) | \
		((Color32)((u8)(g)) << COLOR32_B_SHIFT) | \
		((Color32)((u8)(r)) << COLOR32_A_SHIFT)

	// Color32 Red Channel
#define COLOR32_R(c) \
		((u8)((((Color32)(c)) >> COLOR32_A_SHIFT) & 0xFF))

	// Color32 Green Channel
#define COLOR32_G(c) \
		((u8)((((Color32)(c)) >> COLOR32_B_SHIFT) & 0xFF))

	// Color32 Blue Channel
#define COLOR32_B(c) \
		((u8)((((Color32)(c)) >> COLOR32_B_SHIFT) & 0xFF))

	// Color32 Alpha Channel
#define COLOR32_A(c) \
		((u8)((((Color32)(c)) >> COLOR32_A_SHIFT) & 0xFF))

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct Color
	{
		using storage_type				= Vec4;
		using value_type				= storage_type::value_type;
		using size_type					= storage_type::size_type;
		using difference_type			= storage_type::difference_type;
		using pointer					= storage_type::pointer;
		using reference					= storage_type::reference;
		using const_pointer				= storage_type::const_pointer;
		using const_reference			= storage_type::const_reference;
		using iterator					= storage_type::iterator;
		using const_iterator			= storage_type::const_iterator;
		using reverse_iterator			= storage_type::reverse_iterator;
		using const_reverse_iterator	= storage_type::const_reverse_iterator;

		storage_type m_data{};

		constexpr Color() noexcept = default;
		constexpr Color(Color const &) = default;
		constexpr Color(Color &&) noexcept = default;
		constexpr Color & operator=(Color const &) = default;
		constexpr Color & operator=(Color &&) noexcept = default;
		
		Color & swap(Color & value) noexcept { if (this != std::addressof(value)) { std::swap(m_data, value.m_data); } return (*this); }

		static constexpr bool empty() noexcept { return false; }
		static constexpr auto width() noexcept -> size_t { return 4; }
		static constexpr auto height() noexcept -> size_t { return 1; }
		static constexpr auto size() noexcept -> size_t { return 4; }
		static constexpr auto max_size() noexcept -> size_t { return 4; }

		constexpr auto data() noexcept -> pointer { return m_data; }
		constexpr auto data() const noexcept -> const_pointer { return m_data; }

		constexpr operator pointer() noexcept { return m_data; }
		constexpr operator const_pointer() const noexcept { return m_data; }

		constexpr auto operator*() & noexcept -> reference { return *m_data; }
		constexpr auto operator*() const & noexcept -> const_reference { return *m_data; }
		constexpr auto operator*() && noexcept -> value_type && { return std::move(*m_data); }

		constexpr operator storage_type & () & noexcept { return m_data; }
		constexpr operator storage_type const & () const & noexcept { return m_data; }
		constexpr operator storage_type && () && noexcept { return std::move(m_data); }

		constexpr auto begin() noexcept -> iterator { return m_data.begin(); }
		constexpr auto begin() const noexcept -> const_iterator { return m_data.begin(); }
		constexpr auto cbegin() const noexcept -> const_iterator { return m_data.cbegin(); }
		constexpr auto end() noexcept -> iterator { return m_data.end(); }
		constexpr auto end() const noexcept -> const_iterator { return m_data.end(); }
		constexpr auto cend() const noexcept -> const_iterator { return m_data.cend(); }
		constexpr auto rbegin() noexcept -> reverse_iterator { return m_data.rbegin(); }
		constexpr auto rbegin() const noexcept -> const_reverse_iterator { return m_data.rbegin(); }
		constexpr auto crbegin() const noexcept -> const_reverse_iterator { return m_data.crbegin(); }
		constexpr auto rend() noexcept -> reverse_iterator { return m_data.rend(); }
		constexpr auto rend() const noexcept -> const_reverse_iterator { return m_data.rend(); }
		constexpr auto crend() const noexcept -> const_reverse_iterator { return m_data.crend(); }
	};

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
	
	inline Color rotate_hue(Color const & v, f32 degrees) noexcept
	{
		// https://stackoverflow.com/a/8510751
		f32 const c{ std::cos(util::deg2rad(degrees)) }, s{ std::sin(util::deg2rad(degrees)) };
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
