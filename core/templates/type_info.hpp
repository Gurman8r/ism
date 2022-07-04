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
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if CC_MSVC
#define PRETTY_FUNCTION		__FUNCSIG__
#define PRETTY_TYPE_PREFIX	"class ism::ctti::NameOf __cdecl ism::ctti::nameof<"
#define PRETTY_TYPE_SUFFIX	">(void)"

#elif CC_CLANG
#define PRETTY_FUNCTION		__PRETTY_FUNCTION__
#define PRETTY_TYPE_PREFIX	"ism::ctti::NameOf ism::ctti::nameof() [T = "
#define PRETTY_TYPE_SUFFIX	"]"

#elif CC_GCC
#define PRETTY_FUNCTION		__PRETTY_FUNCTION__
#define PRETTY_TYPE_PREFIX	"constexpr ism::ctti::NameOf ism::ctti::nameof() [with T = "
#define PRETTY_TYPE_SUFFIX	"]"

#else
#error "ctti is available"
#endif

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		class NameOf : public StringView
		{
		public:
			using StringView::StringView;
			using StringView::operator=;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> constexpr NameOf nameof()
		{
			NameOf temp{ PRETTY_FUNCTION };
			temp.remove_prefix(sizeof(PRETTY_TYPE_PREFIX) - 1);
			temp.remove_suffix(sizeof(PRETTY_TYPE_SUFFIX) - 1);
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> constexpr hash_t hash_v{ ctti::nameof<T>().hash_code() };

	template <class T> constexpr hash_t hash() noexcept { return hash_v<T>; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_TYPE_INFO_HPP_
