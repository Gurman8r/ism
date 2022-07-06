#ifndef _ISM_PATH_HPP_
#define _ISM_PATH_HPP_

#include <core/string/string.hpp>

namespace ism::util
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	constexpr bool is_slash(char c) noexcept
	{
		return c == '/' || c == '\\';
	}

	constexpr bool is_drive_prefix(cstring first)
	{
		// test if first points to a prefix of the form X:
		return (first && *first)
			&& ((first[0] >= 'a' && first[0] <= 'z')
			|| (first[0] >= 'A' && first[0] <= 'Z'))
			&& (first[1] == ':');
	}

	constexpr bool has_drive_letter_prefix(cstring first, cstring last)
	{
		return (last - first >= 2) && is_drive_prefix(first);
	}

	inline cstring find_root_name_end(cstring first, cstring last)
	{
		// attempt to parse [first, last) as a path and return the end of root-name if it exists; otherwise, first
		if (last - first < 2) { return first; }

		// check for X: first because it's the most common root-name
		if (has_drive_letter_prefix(first, last)) { return first + 2; }

		// all the other root-names start with a slash; check that first because
		// we expect paths without a leading slash to be very common
		if (!is_slash(first[0])) { return first; }

		// $ means anything other than a slash, including potentially the end of the input
		if (last - first >= 4 && is_slash(first[3]) && (last - first == 4 || !is_slash(first[4])) // \xx\$
			&& ((is_slash(first[1]) && (first[2] == '?' || first[2] == '.')) // \\?\$ or \\.\$
				|| (first[1] == '?' && first[2] == '?'))) { // \??\$
			return first + 3;
		}

		// \\server
		if (last - first >= 3 && is_slash(first[1]) && !is_slash(first[2])) {
			return std::find_if(first + 3, last, is_slash);
		}

		// no match
		return first;
	}

	inline cstring find_relative_path(cstring first, cstring last)
	{
		return std::find_if_not(find_root_name_end(first, last), last, is_slash);
	}

	inline cstring find_filename(cstring first, cstring last)
	{
		auto const relative_path{ find_relative_path(first, last) };
		while (relative_path != last && !is_slash(last[-1])) { --last; }
		return last;
	}

	constexpr cstring find_extension(cstring filename, cstring ads)
	{
		// find dividing point between stem and extension in a generic format filename consisting of [filename, ads)
		auto extension{ ads };

		// empty path
		if (filename == extension) { return ads; }

		--extension;

		// path is length 1 and either dot, or has no dots; either way, extension is empty
		if (filename == extension) { return ads; }

		// we might have found the end of stem
		if (*extension == '.')
		{
			if (filename == extension - 1 && extension[-1] == '.')
			{
				// dotdot special case
				return ads;
			}
			else
			{
				// x.
				return extension;
			}
		}

		while (filename != --extension)
		{
			if (*extension == '.')
			{
				// found a dot which is not in first position, so it starts extension
				return extension;
			}
		}

		// if we got here, either there are no dots, in which case extension is empty, or the first element
		// is a dot, in which case we have the leading single dot special case, which also makes extension empty
		return ads;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline StringView parse_root_name(StringView const value)
	{
		auto const first{ value.data() };
		auto const last{ first + value.size() };
		return { first, static_cast<size_t>(find_root_name_end(first, last) - first) };
	}

	inline StringView parse_root_directory(StringView const value)
	{
		auto const first{ value.data() };
		auto const last{ first + value.size() };
		auto const root_name_end{ find_root_name_end(first, last) };
		auto const relative_path{ std::find_if_not(root_name_end, last, is_slash) };
		return { root_name_end, static_cast<size_t>(relative_path - root_name_end) };
	}

	inline StringView parse_root_path(StringView const value)
	{
		auto const first{ value.data() };
		auto const last{ first + value.size() };
		return { first, static_cast<size_t>(find_relative_path(first, last) - first) };
	}

	inline StringView parse_relative_path(StringView const value)
	{
		auto const first{ value.data() };
		auto const last{ first + value.size() };
		auto const relative_path{ find_relative_path(first, last) };
		return { relative_path, static_cast<size_t>(last - relative_path) };
	}

	inline StringView parse_parent_path(StringView const value)
	{
		auto const first{ value.data() };
		auto last{ first + value.size() };
		auto const relative_path{ find_relative_path(first, last) };
		while (relative_path != last && !is_slash(last[-1])) { --last; }
		while (relative_path != last && is_slash(last[-1])) { --last; }
		return { first, static_cast<size_t>(last - first) };
	}

	inline StringView parse_filename(StringView const value)
	{
		auto const first{ value.data() };
		auto const last{ first + value.size() };
		auto const filename{ find_filename(first, last) };
		return { filename, static_cast<size_t>(last - filename) };
	}

	inline StringView parse_stem(StringView const value)
	{
		auto const first{ value.data() };
		auto const last{ first + value.size() };
		auto const filename{ find_filename(first, last) };
		auto const ads{ std::find(filename, last, ':') };
		auto const extension{ find_extension(filename, ads) };
		return { filename, static_cast<size_t>(extension - filename) };
	}

	inline StringView parse_extension(StringView const value)
	{
		auto const first{ value.data() };
		auto const last{ first + value.size() };
		auto const filename{ find_filename(first, last) };
		auto const ads{ std::find(filename, last, ':') };
		auto const extension{ find_extension(filename, ads) };
		return { extension, static_cast<size_t>(ads - extension) };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Path
	{
		String m_text;

	public:
		using self_type					= typename Path;
		using storage_type				= typename String;
		using view_type					= typename StringView;
		using value_type				= typename storage_type::value_type;
		using pointer					= typename storage_type::pointer;
		using const_pointer				= typename storage_type::const_pointer;
		using reference					= typename storage_type::reference;
		using const_reference			= typename storage_type::const_reference;
		using iterator					= typename storage_type::iterator;
		using const_iterator			= typename storage_type::const_iterator;
		using reverse_iterator			= typename storage_type::reverse_iterator;
		using const_reverse_iterator	= typename storage_type::const_reverse_iterator;
		using size_type					= typename storage_type::size_type;
		using difference_type			= typename storage_type::difference_type;

	public:
		Path() noexcept : m_text{} {}
		Path(nullptr_t) : m_text{} {}
		Path(const_pointer value) : m_text{ value } {}
		Path(const_pointer value, size_type size) : m_text{ value, size } {}
		Path(const_pointer first, const_pointer last) : m_text{ first, last } {}
		Path(storage_type const & value) : m_text{ value } {}
		Path(storage_type && value) noexcept : m_text{ std::move(value) } {}
		Path(view_type const value) : m_text{ value.data(), value.size() } {}
		Path(self_type const & value) : m_text{ value } {}
		Path(self_type && value) noexcept : m_text{ std::move(value.m_text) } {}
		self_type & operator=(self_type const & value) { return copy(value); }

	public:
		self_type & clear() noexcept { return m_text.clear(), (*this); }
		self_type & copy(self_type const & value) noexcept { return (m_text = value.m_text), (*this); }
		self_type & swap(self_type & value) noexcept { return m_text.swap(value.m_text), (*this); }

	public:
		operator void * () const noexcept { return m_text.operator void * (); }
		bool operator==(storage_type const & value) { return m_text == value; }
		bool operator==(const_pointer value) { return m_text == value; }
		bool operator!=(storage_type const & value) { return m_text != value; }
		bool operator<(self_type const & value) { return m_text < value.m_text; }
		bool operator>(self_type const & value) { return m_text > value.m_text; }
		bool operator==(self_type const & value) { return m_text == value.m_text; }

		auto data() noexcept -> pointer { return m_text.data(); }
		auto data() const noexcept -> const_pointer { return m_text.data(); }
		auto c_str() const noexcept -> const_pointer { return m_text.data(); }
		bool empty() const noexcept { return m_text.empty(); }
		auto length() const noexcept -> size_type { return m_text.size(); }
		auto size() const noexcept -> size_type { return m_text.size(); }
		auto hash_code() const noexcept -> hash_t { return m_text.hash_code(); }

		char & operator[](size_type i) & noexcept { return m_text[i]; }
		char operator[](size_type i) const & noexcept { return m_text[i]; }

		auto view() const noexcept -> view_type { return { m_text.data(), m_text.size() }; }
		auto string() & noexcept -> storage_type & { return m_text; }
		auto string() const & noexcept -> storage_type const & { return m_text; }
		auto string() && noexcept -> storage_type && { return std::move(m_text); }

		operator view_type () const noexcept { return { m_text.data(), m_text.size() }; }
		operator storage_type & () & noexcept { return m_text; }
		operator storage_type const & () const & noexcept { return m_text; }
		operator storage_type && () && noexcept { return std::move(m_text); }

	public:
		auto root_name() const noexcept -> self_type { return util::parse_root_name(view()); }
		auto root_directory() const noexcept -> self_type { return util::parse_root_directory(view()); }
		auto root_path() const noexcept -> self_type { return util::parse_root_path(view()); }
		auto relative_path() const noexcept -> self_type { return util::parse_relative_path(view()); }
		auto parent_path() const noexcept -> self_type { return util::parse_parent_path(view()); }
		auto filename() const noexcept -> self_type { return util::parse_filename(view()); }
		auto stem() const noexcept -> self_type { return util::parse_stem(view()); }
		auto extension() const noexcept -> self_type { return util::parse_extension(view()); }

		bool has_root_name() const noexcept { return !util::parse_root_name(view()).empty(); }
		bool has_root_directory() const noexcept { return !util::parse_root_directory(view()).empty(); }
		bool has_root_path() const noexcept { return !util::parse_root_path(view()).empty(); }
		bool has_relative_path() const noexcept { return !util::parse_relative_path(view()).empty(); }
		bool has_parent_path() const noexcept { return !util::parse_parent_path(view()).empty(); }
		bool has_filename() const noexcept { return !util::parse_filename(view()).empty(); }
		bool has_stem() const noexcept { return !util::parse_stem(view()).empty(); }
		bool has_extension() const noexcept { return !util::parse_extension(view()).empty(); }

	public:
		auto begin() noexcept -> iterator { return m_text.begin(); }
		auto begin() const noexcept -> const_iterator { return m_text.begin(); }
		auto cbegin() const noexcept -> const_iterator { return m_text.cbegin(); }

		auto end() noexcept -> iterator { return m_text.end(); }
		auto end() const noexcept -> const_iterator { return m_text.end(); }
		auto cend() const noexcept -> const_iterator { return m_text.cend(); }

		auto rbegin() noexcept -> reverse_iterator { return m_text.rbegin(); }
		auto rbegin() const noexcept -> const_reverse_iterator { return m_text.rbegin(); }
		auto crbegin() const noexcept -> const_reverse_iterator { return m_text.crbegin(); }

		auto rend() noexcept -> reverse_iterator { return m_text.rend(); }
		auto rend() const noexcept -> const_reverse_iterator { return m_text.rend(); }
		auto crend() const noexcept -> const_reverse_iterator { return m_text.crend(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace mpl { template <> constexpr bool is_string_v<Path>{ true }; }

	inline Path operator "" _path(cstring s, size_t n) noexcept { return Path{ String{ s, n } }; }

	template <> struct Hasher<Path>
	{
		hash_t operator()(Path const & value) const { return value.hash_code(); }
	};

	inline bool operator==(String const & lhs, Path const & rhs) { return lhs == rhs.string(); }

	inline bool operator!=(String const & lhs, Path const & rhs) { return lhs != rhs.string(); }

	inline bool operator==(cstring lhs, Path const & rhs) { return lhs == rhs.string(); }

	inline bool operator!=(cstring lhs, Path const & rhs) { return lhs != rhs.string(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_PATH_HPP_
