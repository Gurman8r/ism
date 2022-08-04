#ifndef _ISM_TYPEDEFS_HPP_
#define _ISM_TYPEDEFS_HPP_

#include <core/version.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// alias
#define ALIAS(m_type) \
		using m_type = 

// opaque type
#define OPAQUE_TYPE(m_name) \
		struct CAT(__, m_name) { int unused; }; \
		ALIAS(m_name) CAT(__, m_name) *;

// strong typedef
#define STRONG_TYPEDEF(m_to, m_from)																					\
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
		}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef CC_MSVC
	ALIAS(i8)			signed __int8;
	ALIAS(i16)			signed __int16;
	ALIAS(i32)			signed __int32;
	ALIAS(i64)			signed __int64;
	ALIAS(u8)			unsigned __int8;
	ALIAS(u16)			unsigned __int16;
	ALIAS(u32)			unsigned __int32;
	ALIAS(u64)			unsigned __int64;
#else
	ALIAS(i8)			signed char;
	ALIAS(i16)			signed short;
	ALIAS(i32)			signed int;
	ALIAS(i64)			signed long long;
	ALIAS(u8)			unsigned char;
	ALIAS(u16)			unsigned short;
	ALIAS(u32)			unsigned int;
	ALIAS(u64)			unsigned long long;
#endif

	ALIAS(f32)			float;
	ALIAS(f64)			double;
	ALIAS(f80)			long double;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if (ARCHITECTURE == 64)
	ALIAS(ssize_t)		i64;
	ALIAS(size_t)		u64;
#else
	ALIAS(ssize_t)		i32;
	ALIAS(size_t)		u32;
#endif

	ALIAS(intptr_t)		ssize_t;
	ALIAS(uintptr_t)	size_t;
	ALIAS(ptrdiff_t)	ssize_t;
	ALIAS(nullptr_t)	decltype(nullptr);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ALIAS(cstring)		char const *;
	ALIAS(cwstring)		wchar_t const *;
#if (HAS_CXX_20)
	ALIAS(c8string)		char8_t const *;
#else
	ALIAS(c8string)		char const *;
#endif
	ALIAS(c16string)	char16_t const *;
	ALIAS(c32string)	char32_t const *;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* ALIGNMENT HELPERS */

// round down size m_size to be a multiple of m_alignment, where m_alignment is a power of 2
#define SIZE_ROUND_DOWN(m_size, m_alignment) \
		((size_t)(m_size) & ~(size_t)((m_alignment) - 1))

// round up size m_size to be m_alignment multiple of m_alignment, where m_alignment is a power of 2
#define SIZE_ROUND_UP(m_size, m_alignment) \
		(((size_t)(m_size) + (size_t)((m_alignment) - 1)) & ~(size_t)((m_alignment) - 1))

// round pointer m_ptr down to the closest m_alignment-aligned address <= m_ptr, where m_alignment is a power of 2
#define ALIGN_DOWN(m_ptr, m_alignment) \
		((void *)((uintptr_t)(m_ptr) & ~(uintptr_t)((m_alignment) - 1)))

// round pointer m_ptr up to the closest m_alignment-aligned address >= m_ptr, where m_alignment is a power of 2
#define ALIGN_UP(m_ptr, m_alignment) \
		((void *)(((uintptr_t)(m_ptr) + (uintptr_t)((m_alignment) - 1)) & ~(uintptr_t)((m_alignment) - 1)))

// check if pointer m_ptr is aligned to m_alignment-bytes boundary, where m_alignment is a power of 2
#define IS_ALIGNED(m_ptr, m_alignment) \
		(!((uintptr_t)(m_ptr) & (uintptr_t)((m_alignment) - 1)))

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ISM_TYPEDEFS_HPP_
