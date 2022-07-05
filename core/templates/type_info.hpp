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

#if ISM_CC_MSVC
#define PRETTY_FUNCTION		__FUNCSIG__
#define PRETTY_TYPE_PREFIX	"class ism::ctti::NameView __cdecl ism::ctti::nameof<"
#define PRETTY_TYPE_SUFFIX	">(void)"
#elif ISM_CC_CLANG
#define PRETTY_FUNCTION		__PRETTY_FUNCTION__
#define PRETTY_TYPE_PREFIX	"ism::ctti::NameView ism::ctti::nameof() [T = "
#define PRETTY_TYPE_SUFFIX	"]"
#elif ISM_CC_GCC
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

	// nameof
	template <class T> constexpr StringView nameof_v
	{
		ctti::nameof<T>()
	};

	// hashof
	template <class T> constexpr hash_t hashof_v
	{
		nameof_v<T>.hash_code()
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_TYPE_INFO_HPP_
