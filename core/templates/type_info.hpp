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

		// non-template view type needed ahead
		class NameView : public StringView {
		public:
			using StringView::StringView;
			using StringView::operator=;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if CC_MSVC
#define PRETTY_FUNCTION		__FUNCSIG__
#define PRETTY_TYPE_PREFIX	"class ism::ctti::NameView __cdecl ism::ctti::nameof<"
#define PRETTY_TYPE_SUFFIX	">(void)"
#elif CC_CLANG
#define PRETTY_FUNCTION		__PRETTY_FUNCTION__
#define PRETTY_TYPE_PREFIX	"ism::ctti::NameView ism::ctti::nameof() [T = "
#define PRETTY_TYPE_SUFFIX	"]"
#elif CC_GCC
#define PRETTY_FUNCTION		__PRETTY_FUNCTION__
#define PRETTY_TYPE_PREFIX	"constexpr ism::ctti::NameView ism::ctti::nameof() [with T = "
#define PRETTY_TYPE_SUFFIX	"]"
#else
#error "ctti is unavailable"
#endif

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> constexpr NameView nameof()
		{
			NameView temp{ PRETTY_FUNCTION };
			temp.remove_prefix(sizeof(PRETTY_TYPE_PREFIX) - 1);
			temp.remove_suffix(sizeof(PRETTY_TYPE_SUFFIX) - 1);
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// nameof_t
	template <class T
	> struct nameof_t {
		static constexpr StringView value{ ctti::nameof<T>() };
	};

	// nameof_v
	template <class T
	> constexpr StringView nameof_v{
		nameof_t<T>::value
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// hashof_t
	template <class T
	> struct hashof_t {
		static constexpr size_t value{ nameof_v<T>.hash_code() };
	};

	// hashof_v
	template <class T
	> constexpr size_t hashof_v{
		hashof_t<T>::value
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_TYPE_INFO_HPP_
