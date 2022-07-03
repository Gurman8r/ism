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
#if defined(CC_MSVC)
#	define PRETTY_FUNCTION		__FUNCSIG__
#	define PRETTY_TYPE_PREFIX	"class ism::StringView __cdecl ism::ctti::name<"
#	define PRETTY_TYPE_SUFFIX	">(void)"

#elif defined(CC_CLANG)
#	define PRETTY_FUNCTION		__PRETTY_FUNCTION__
#	define PRETTY_TYPE_PREFIX	"ism::StringView ism::ctti::name() [T = "
#	define PRETTY_TYPE_SUFFIX	"]"

#elif defined(CC_GCC)
#	define PRETTY_FUNCTION		__PRETTY_FUNCTION__
#	define PRETTY_TYPE_PREFIX	"constexpr ism::StringView ism::ctti::name() [with T = "
#	define PRETTY_TYPE_SUFFIX	"]"

#else
#	error "Type information is not available."
#endif
		template <class T> constexpr StringView name()
		{
			return StringView{ PRETTY_FUNCTION }
				.remove_prefix(sizeof(PRETTY_TYPE_PREFIX) - 1)
				.remove_suffix(sizeof(PRETTY_TYPE_SUFFIX) - 1);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> constexpr hash_t hash_v{ hash(ctti::name<T>()) };

	template <class T> constexpr hash_t hash() noexcept { return hash_v<T>; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_TYPE_INFO_HPP_
