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
#if defined(ISM_CC_MSVC)
#	define PRETTY_FUNCTION		__FUNCSIG__
#	define PRETTY_TYPE_PREFIX	"class ism::StringView __cdecl ism::ctti::raw<"
#	define PRETTY_TYPE_SUFFIX	">(void)"
#elif defined(ISM_CC_CLANG)
#	define PRETTY_FUNCTION		__PRETTY_FUNCTION__
#	define PRETTY_TYPE_PREFIX	"ism::StringView ism::ctti::raw() [T = "
#	define PRETTY_TYPE_SUFFIX	"]"
#elif defined(ISM_CC_GCC)
#	define PRETTY_FUNCTION		__PRETTY_FUNCTION__
#	define PRETTY_TYPE_PREFIX	"constexpr ism::StringView ism::ctti::raw() [with T = "
#	define PRETTY_TYPE_SUFFIX	"]"
#else
#	error TypeObject information is not available.
#endif
		template <class T> NODISCARD static constexpr StringView raw()
		{
			return StringView{ PRETTY_FUNCTION }
				.remove_prefix(sizeof(PRETTY_TYPE_PREFIX) - 1)
				.remove_suffix(sizeof(PRETTY_TYPE_SUFFIX) - 1);
		}

		template <class T> constexpr StringView type_v{ ctti::raw<T>() };

		template <class T> NODISCARD constexpr StringView type()
		{
			return ctti::type_v<T>;
		}

		template <class T> NODISCARD constexpr hash_t hash()
		{
			return ism::hash(ctti::type<T>());
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// run time type info
	namespace rtti
	{
		static String & filter(String & name)
		{
			mpl::for_args([&name](auto search)
			{
				for (size_t pos = 0;;)
				{
					pos = name.find(name, pos);
					if (pos == String::npos) { break; }
					name.erase(pos, name.length());
				}
			}
			, "class ", "struct ", "enum ");
			return name;
		}

		template <class T> NODISCARD String type() noexcept
		{
			String name{ typeid(T).name() };
			return rtti::filter(name);
		}

		template <class T> NODISCARD hash_t hash() noexcept
		{
			return ism::hash(rtti::type<T>());
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline bool same_type(std::type_info const & lhs, std::type_info const & rhs)
	{
		return lhs.name() == rhs.name() || std::strcmp(lhs.name(), rhs.name()) == 0;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> NODISCARD constexpr hash_t hash() noexcept { return ctti::hash<T>(); }

	template <class T> constexpr hash_t hashof_v{ ism::hash<T>() };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct ism::EqualTo<std::type_index>
	{
		bool operator()(std::type_index const & lhs, std::type_index const & rhs) const
		{
			return lhs.name() == rhs.name() || std::strcmp(lhs.name(), rhs.name()) == 0;
		}
	};

	template <> struct ism::Hash<std::type_index>
	{
		hash_t operator()(std::type_index const & t) const
		{
			return ism::Hash<String>()(t.name());
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_TYPE_INFO_HPP_
