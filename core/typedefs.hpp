#ifndef _ISM_TYPEDEFS_HPP_
#define _ISM_TYPEDEFS_HPP_

#include <core/version.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// root namespace
namespace ism {}

// private namespace
namespace ism::priv {}

// utilities namespace
namespace ism::util {}

// meta programming language
namespace ism::mpl {}

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
	ALIAS(sbyte)		signed __int8;
	ALIAS(i16)			signed __int16;
	ALIAS(i32)			signed __int32;
	ALIAS(i64)			signed __int64;
	ALIAS(byte)			unsigned __int8;
	ALIAS(u16)			unsigned __int16;
	ALIAS(u32)			unsigned __int32;
	ALIAS(u64)			unsigned __int64;
#else
	ALIAS(sbyte)		signed char;
	ALIAS(i16)			signed short;
	ALIAS(i32)			signed int;
	ALIAS(i64)			signed long long;
	ALIAS(byte)			unsigned char;
	ALIAS(u16)			unsigned short;
	ALIAS(u32)			unsigned int;
	ALIAS(u64)			unsigned long long;
#endif

	ALIAS(f32)			float;
	ALIAS(f64)			double;
	ALIAS(f80)			long double;

#if (ARCHITECTURE == 64)
	ALIAS(ssize_t)		i64;
	ALIAS(size_t)		u64;
#else
	ALIAS(ssize_t)		i32;
	ALIAS(size_t)		u32;
#endif

	ALIAS(intptr_t)		ssize_t;
	ALIAS(ptrdiff_t)	ssize_t;
	ALIAS(hash_t)		size_t;
	ALIAS(nullptr_t)	decltype(nullptr);

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
}

#endif // !_ISM_TYPEDEFS_HPP_
