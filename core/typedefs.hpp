#ifndef _ISM_TYPEDEFS_HPP_
#define _ISM_TYPEDEFS_HPP_

#include <core/version.hpp>

#include <cstddef>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// alias
#define ALIAS(m_type) \
	using m_type = 

// strong typedef
#define STRONG_TYPEDEF(m_to, m_from)																			\
class m_to final {																								\
private:																										\
	m_from value;																								\
public:																											\
	inline m_to() = default;																					\
	inline m_to(m_to const & other) = default;																	\
	inline m_to(m_to && other) = default;																		\
	inline m_to & operator=(m_to const & other) = default;														\
	inline m_to & operator=(m_to && other) = default;															\
	inline constexpr explicit m_to(m_from other) noexcept : value{ other } {}									\
	inline constexpr m_to & operator=(m_from other) noexcept { value = other; return (*this); }					\
	inline constexpr operator m_from const & () const noexcept { return value; }								\
	inline constexpr operator m_from & () noexcept { return value; }											\
	inline constexpr decltype(auto) operator==(m_to const & other) noexcept { return value == other.value; }	\
	inline constexpr decltype(auto) operator!=(m_to const & other) noexcept { return value != other.value; }	\
	inline constexpr decltype(auto) operator<(m_to const & other) noexcept { return value < other.value; }		\
	inline constexpr decltype(auto) operator>(m_to const & other) noexcept { return value > other.value; }		\
	inline constexpr decltype(auto) operator<=(m_to const & other) noexcept { return value <= other.value; }	\
	inline constexpr decltype(auto) operator>=(m_to const & other) noexcept { return value >= other.value; }	\
}																												\

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// declare opaque handle type
#define DECL_HANDLE(m_name) \
	struct CAT(__, m_name) { int unused; }; \
	ALIAS(m_name) CAT(__, m_name) *

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// declare tag object type
#define DECL_TAG(m_type, m_var) \
	struct m_type { struct _Tag {}; constexpr explicit m_type(_Tag) {} }; \
	inline constexpr m_type m_var{ m_type::_Tag{} }; \

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct void_type {};

#ifdef CC_MSVC
	ALIAS(int8_t)		signed __int8;
	ALIAS(int16_t)		signed __int16;
	ALIAS(int32_t)		signed __int32;
	ALIAS(int64_t)		signed __int64;
	ALIAS(uint8_t)		unsigned __int8;
	ALIAS(uint16_t)		unsigned __int16;
	ALIAS(uint32_t)		unsigned __int32;
	ALIAS(uint64_t)		unsigned __int64;
#else
	ALIAS(int8_t)		signed char;
	ALIAS(int16_t)		signed short;
	ALIAS(int32_t)		signed int;
	ALIAS(int64_t)		signed long long;
	ALIAS(uint8_t)		unsigned char;
	ALIAS(uint16_t)		unsigned short;
	ALIAS(uint32_t)		unsigned int;
	ALIAS(uint64_t)		unsigned long long;
#endif

	ALIAS(signed_t)		signed;
	ALIAS(unsigned_t)	unsigned;
	ALIAS(slong_t)		signed long;
	ALIAS(ulong_t)		unsigned long;
	ALIAS(float32_t)	float;
	ALIAS(float64_t)	double;
	ALIAS(float80_t)	long double;
	ALIAS(nullptr_t)	decltype(nullptr);

	ALIAS(float_t)		float32_t;
	ALIAS(double_t)		float64_t;

#if (ARCHITECTURE == 32)
	ALIAS(intmax_t)		int32_t;
	ALIAS(uintmax_t)	uint32_t;
#else
	ALIAS(intmax_t)		int64_t;
	ALIAS(uintmax_t)	uint64_t;
#endif

	ALIAS(max_align_t)	double_t;
	ALIAS(hash_t)		uintmax_t;
	ALIAS(intptr_t)		intmax_t;
	ALIAS(ptrdiff_t)	intmax_t;
	ALIAS(ssize_t)		intmax_t;
	ALIAS(size_t)		uintmax_t;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ALIAS(cstring)		char const *;
	ALIAS(cwstring)		wchar_t const *;
#if (CXX_20)
	ALIAS(c8string)		char8_t const *;
#else
	ALIAS(c8string)		char const *;
#endif
	ALIAS(c16string)	char16_t const *;
	ALIAS(c32string)	char32_t const *;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum class byte : uint8_t
	{
		zero, one,
		min = 0, null = min,
		max = 255, npos = max
	};

	template <class I, std::enable_if_t<std::is_integral_v<I>, int> = 0
	> NODISCARD constexpr byte operator<<(byte const a, I const b) noexcept { return static_cast<byte>(static_cast<uint8_t>(static_cast<uint32_t>(a) << b)); }

	template <class I, std::enable_if_t<std::is_integral_v<I>, int> = 0
	> NODISCARD constexpr byte operator>>(byte const a, I const b) noexcept { return static_cast<byte>(static_cast<uint8_t>(static_cast<uint32_t>(a) >> b)); }

	NODISCARD constexpr byte operator|(byte const a, byte const b) noexcept { return static_cast<byte>(static_cast<uint8_t>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b))); }

	NODISCARD constexpr byte operator&(byte const a, byte const b) noexcept { return static_cast<byte>(static_cast<uint8_t>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b))); }

	NODISCARD constexpr byte operator^(byte const a, byte const b) noexcept { return static_cast<byte>(static_cast<uint8_t>(static_cast<uint32_t>(a) ^ static_cast<uint32_t>(b))); }

	NODISCARD constexpr byte operator~(byte const a) noexcept { return static_cast<byte>(static_cast<uint8_t>(~static_cast<uint32_t>(a))); }

	template <class I, std::enable_if_t<std::is_integral_v<I>, int> = 0
	> constexpr byte & operator<<=(byte & a, I const b) noexcept { return a = a << b; }

	template <class I, std::enable_if_t<std::is_integral_v<I>, int> = 0
	> constexpr byte & operator>>=(byte & a, I const b) noexcept { return a = a >> b; }

	constexpr byte & operator|=(byte & a, byte const b) noexcept { return a = a | b; }

	constexpr byte & operator&=(byte & a, byte const b) noexcept { return a = a & b; }

	constexpr byte & operator^=(byte & a, byte const b) noexcept { return a = a ^ b; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_TYPEDEFS_HPP_
