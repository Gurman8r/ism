#ifndef _ISM_TYPE_INFO_HPP_
#define _ISM_TYPE_INFO_HPP_

#include <core/string/string_view.hpp>

#include <typeinfo>
#include <typeindex>

namespace ISM
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// compile time type info
	namespace CTTI
	{
#if defined(ISM_CC_MSVC)
#	define PRETTY_FUNCTION		__FUNCSIG__
#	define PRETTY_TYPE_PREFIX	"class ISM::StringView __cdecl ISM::CTTI::raw<"
#	define PRETTY_TYPE_SUFFIX	">(void)"
#elif defined(ISM_CC_CLANG)
#	define PRETTY_FUNCTION		__PRETTY_FUNCTION__
#	define PRETTY_TYPE_PREFIX	"ISM::StringView ISM::CTTI::raw() [T = "
#	define PRETTY_TYPE_SUFFIX	"]"
#elif defined(ISM_CC_GCC)
#	define PRETTY_FUNCTION		__PRETTY_FUNCTION__
#	define PRETTY_TYPE_PREFIX	"constexpr ISM::StringView ISM::CTTI::raw() [with T = "
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

		template <class T> constexpr StringView type_v{ CTTI::raw<T>() };

		template <class T> NODISCARD static constexpr StringView type()
		{
			return CTTI::type_v<T>;
		}

		template <class T> NODISCARD constexpr hash_t hash()
		{
			return ISM::hashof(CTTI::type<T>());
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// run time type info
	namespace RTTI
	{
		ALIAS(Index)	std::type_index;
		ALIAS(Info)		std::type_info;

		static String & filter(String & name)
		{
			MPL::for_args([&name](auto search)
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

		template <class T> static String type()
		{
			String name{ typeid(T).name() };
			return RTTI::filter(name);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> NODISCARD constexpr hash_t hashof() noexcept { return CTTI::hash<T>(); }

	template <class T> constexpr hash_t hashof_v{ ISM::hashof<T>() };

	inline bool same_type(RTTI::Info const & lhs, RTTI::Info const & rhs)
	{
		return lhs.name() == rhs.name() || std::strcmp(lhs.name(), rhs.name()) == 0;
	}

	template <> struct ISM::EqualTo<RTTI::Index>
	{
		bool operator()(RTTI::Index const & lhs, RTTI::Index const & rhs) const
		{
			return lhs.name() == rhs.name() || std::strcmp(lhs.name(), rhs.name()) == 0;
		}
	};

	template <> struct ISM::Hash<RTTI::Index>
	{
		hash_t operator()(RTTI::Index const & t) const
		{
			return ISM::Hash<String>()(t.name());
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_TYPE_INFO_HPP_
