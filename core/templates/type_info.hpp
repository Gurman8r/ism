#ifndef _ISM_TYPE_INFO_HPP_
#define _ISM_TYPE_INFO_HPP_

#include <core/string/string_view.hpp>

#include <typeinfo>
#include <typeindex>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// compile time type info
	namespace ctti
	{
#if defined(COMPILER_MSVC)
#	define PRETTY_FUNCTION		__FUNCSIG__
#	define PRETTY_TYPE_PREFIX	"class ism::StringView __cdecl ism::ctti::type<"
#	define PRETTY_TYPE_SUFFIX	">(void)"

#elif defined(COMPILER_CLANG)
#	define PRETTY_FUNCTION		__PRETTY_FUNCTION__
#	define PRETTY_TYPE_PREFIX	"ism::StringView ism::ctti::type() [T = "
#	define PRETTY_TYPE_SUFFIX	"]"

#elif defined(COMPILER_GCC)
#	define PRETTY_FUNCTION		__PRETTY_FUNCTION__
#	define PRETTY_TYPE_PREFIX	"constexpr ism::StringView ism::ctti::type() [with T = "
#	define PRETTY_TYPE_SUFFIX	"]"

#else
#	error type information is unavailable
#endif
		template <class T> NODISCARD constexpr StringView type()
		{
			return StringView{ PRETTY_FUNCTION }
				.remove_prefix(sizeof(PRETTY_TYPE_PREFIX) - 1)
				.remove_suffix(sizeof(PRETTY_TYPE_SUFFIX) - 1);
		}

		template <class T> constexpr StringView type_v{ ctti::type<T>() };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> NODISCARD constexpr hash_t hash() noexcept { return ism::hash(ctti::type_v<T>); }

	template <class T> constexpr hash_t hash_v{ ism::hash<T>() };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_TYPE_INFO_HPP_
