#ifndef _ISM_PATH_HPP_
#define _ISM_PATH_HPP_

#include <core/string/string.hpp>

namespace ism::util
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD constexpr bool is_slash(char c) noexcept
	{
		return c == '/' || c == '\\';
	}

	NODISCARD constexpr bool is_drive_prefix(cstring first)
	{
		// test if first points to a prefix of the form X:
		return (first && *first)
			&& ((first[0] >= 'a' && first[0] <= 'z')
			|| (first[0] >= 'A' && first[0] <= 'Z'))
			&& (first[1] == ':');
	}

	NODISCARD constexpr bool has_drive_letter_prefix(cstring first, cstring last)
	{
		return (last - first >= 2) && is_drive_prefix(first);
	}

	NODISCARD inline cstring find_root_name_end(cstring first, cstring last)
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

	NODISCARD inline cstring find_relative_path(cstring first, cstring last)
	{
		return std::find_if_not(find_root_name_end(first, last), last, is_slash);
	}

	NODISCARD inline cstring find_filename(cstring first, cstring last)
	{
		auto const relative_path{ find_relative_path(first, last) };
		while (relative_path != last && !is_slash(last[-1])) { --last; }
		return last;
	}

	NODISCARD constexpr cstring find_extension(cstring filename, cstring ads)
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

	NODISCARD inline StringView parse_root_name(StringView const value)
	{
		auto const first{ value.data() };
		auto const last{ first + value.size() };
		return { first, static_cast<size_t>(find_root_name_end(first, last) - first) };
	}

	NODISCARD inline StringView parse_root_directory(StringView const value)
	{
		auto const first{ value.data() };
		auto const last{ first + value.size() };
		auto const root_name_end{ find_root_name_end(first, last) };
		auto const relative_path{ std::find_if_not(root_name_end, last, is_slash) };
		return { root_name_end, static_cast<size_t>(relative_path - root_name_end) };
	}

	NODISCARD inline StringView parse_root_path(StringView const value)
	{
		auto const first{ value.data() };
		auto const last{ first + value.size() };
		return { first, static_cast<size_t>(find_relative_path(first, last) - first) };
	}

	NODISCARD inline StringView parse_relative_path(StringView const value)
	{
		auto const first{ value.data() };
		auto const last{ first + value.size() };
		auto const relative_path{ find_relative_path(first, last) };
		return { relative_path, static_cast<size_t>(last - relative_path) };
	}

	NODISCARD inline StringView parse_parent_path(StringView const value)
	{
		auto const first{ value.data() };
		auto last{ first + value.size() };
		auto const relative_path{ find_relative_path(first, last) };
		while (relative_path != last && !is_slash(last[-1])) { --last; }
		while (relative_path != last && is_slash(last[-1])) { --last; }
		return { first, static_cast<size_t>(last - first) };
	}

	NODISCARD inline StringView parse_filename(StringView const value)
	{
		auto const first{ value.data() };
		auto const last{ first + value.size() };
		auto const filename{ find_filename(first, last) };
		return { filename, static_cast<size_t>(last - filename) };
	}

	NODISCARD inline StringView parse_stem(StringView const value)
	{
		auto const first{ value.data() };
		auto const last{ first + value.size() };
		auto const filename{ find_filename(first, last) };
		auto const ads{ std::find(filename, last, ':') };
		auto const extension{ find_extension(filename, ads) };
		return { filename, static_cast<size_t>(extension - filename) };
	}

	NODISCARD inline StringView parse_extension(StringView const value)
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

		self_type & swap(self_type & value) noexcept { return m_text.swap(value.m_text), (*this); }

		void clear() noexcept { m_text.clear(); }

		NODISCARD operator void * () const noexcept { return !empty() ? (void *)data() : nullptr; }
		NODISCARD auto data() noexcept -> pointer { return m_text.data(); }
		NODISCARD auto data() const noexcept -> const_pointer { return m_text.data(); }
		NODISCARD auto c_str() const noexcept -> const_pointer { return m_text.data(); }
		NODISCARD bool empty() const noexcept { return m_text.empty(); }
		NODISCARD auto length() const noexcept -> size_type { return m_text.size(); }
		NODISCARD auto size() const noexcept -> size_type { return m_text.size(); }
		NODISCARD auto hash_code() const noexcept -> size_type { return m_text.hash_code(); }
		NODISCARD char & operator[](size_type i) & noexcept { return m_text[i]; }
		NODISCARD char operator[](size_type i) const & noexcept { return m_text[i]; }

		NODISCARD auto string() & noexcept -> storage_type & { return m_text; }
		NODISCARD auto string() const & noexcept -> storage_type const & { return m_text; }
		NODISCARD auto string() && noexcept -> storage_type && { return std::move(m_text); }
		NODISCARD auto view() const noexcept -> view_type { return { m_text.data(), m_text.size() }; }

		NODISCARD operator storage_type & () & noexcept { return m_text; }
		NODISCARD operator storage_type const & () const & noexcept { return m_text; }
		NODISCARD operator storage_type && () && noexcept { return std::move(m_text); }
		NODISCARD operator view_type () const noexcept { return { m_text.data(), m_text.size() }; }

		NODISCARD auto root_name() const noexcept -> self_type { return util::parse_root_name(view()); }
		NODISCARD auto root_directory() const noexcept -> self_type { return util::parse_root_directory(view()); }
		NODISCARD auto root_path() const noexcept -> self_type { return util::parse_root_path(view()); }
		NODISCARD auto relative_path() const noexcept -> self_type { return util::parse_relative_path(view()); }
		NODISCARD auto parent_path() const noexcept -> self_type { return util::parse_parent_path(view()); }
		NODISCARD auto filename() const noexcept -> self_type { return util::parse_filename(view()); }
		NODISCARD auto stem() const noexcept -> self_type { return util::parse_stem(view()); }
		NODISCARD auto extension() const noexcept -> self_type { return util::parse_extension(view()); }

		NODISCARD bool has_root_name() const noexcept { return !util::parse_root_name(view()).empty(); }
		NODISCARD bool has_root_directory() const noexcept { return !util::parse_root_directory(view()).empty(); }
		NODISCARD bool has_root_path() const noexcept { return !util::parse_root_path(view()).empty(); }
		NODISCARD bool has_relative_path() const noexcept { return !util::parse_relative_path(view()).empty(); }
		NODISCARD bool has_parent_path() const noexcept { return !util::parse_parent_path(view()).empty(); }
		NODISCARD bool has_filename() const noexcept { return !util::parse_filename(view()).empty(); }
		NODISCARD bool has_stem() const noexcept { return !util::parse_stem(view()).empty(); }
		NODISCARD bool has_extension() const noexcept { return !util::parse_extension(view()).empty(); }

		WRAP_ALL_ITERATORS(m_text);

	public:
		static constexpr size_t max_path{ 260 };

		static i32 format(self_type & s, cstring fmt, va_list args) {
			char buffer[max_path]{};
			i32 const n{ std::vsnprintf(buffer, max_path, fmt, args) };
			if (0 < n) {
				s.m_text = storage_type{ buffer, buffer + (size_t)n, storage_type::allocator_type{} };
			}
			return n;
		}

		static i32 format(self_type & s, cstring fmt, ...) noexcept {
			va_list args;
			va_start(args, fmt);
			i32 const n{ format(s, fmt, args) };
			va_end(args);
			return n;
		}

		NODISCARD static self_type format(cstring fmt, va_list args) noexcept {
			self_type s;
			format(s, fmt, args);
			return s;
		}

		NODISCARD static self_type format(cstring fmt, ...) noexcept {
			va_list args;
			va_start(args, fmt);
			auto const s{ format(fmt, args) };
			va_end(args);
			return s;
		}

	public:
		self_type & operator=(self_type const & value) { return (m_text = value.m_text), (*this); }
		//self_type & operator+=(self_type const & value) { return (m_text += value.m_text), (*this); }
		//self_type & operator+=(self_type && value) noexcept { return (m_text += std::move(value.m_text)), (*this); }
		//template <class T> self_type & operator+=(T const & value) { return (m_text += value), (*this); }
		//template <class T> self_type & operator+=(T && value) noexcept { return (m_text += std::move(value)), (*this); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace mpl { template <> constexpr bool is_string_v<Path>{ true }; }

	inline Path operator "" _path(cstring s, size_t n) noexcept { return Path{ String{ s, n } }; }

	template <> struct Hasher<Path>
	{
		size_t operator()(Path const & value) const { return value.hash_code(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	//template <class T> NODISCARD Path operator+(Path const & a, T const & b) { return Path{ a.string() + b }; }
	//template <class T> NODISCARD Path operator+(T const & a, Path const & b) { return Path{ a + b.string() }; }
	//template <class T> NODISCARD Path operator+(Path && a, T const & b) noexcept { return Path{ std::move(a).string() + b }; }
	//template <class T> NODISCARD Path operator+(T const & a, Path && b) noexcept { return Path{ a + std::move(b).string() }; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline bool operator==(String const & lhs, Path const & rhs) { return lhs == rhs.string(); }
	inline bool operator==(cstring lhs, Path const & rhs) { return lhs == rhs.string(); }
	inline bool operator==(Path const & lhs, cstring rhs) { return lhs.string() == rhs; }
	inline bool operator==(Path const & lhs, Path const & rhs) { return lhs.string() == rhs.string(); }

	inline bool operator!=(String const & lhs, Path const & rhs) { return lhs != rhs.string(); }
	inline bool operator!=(cstring lhs, Path const & rhs) { return lhs != rhs.string(); }
	inline bool operator!=(Path const & lhs, cstring rhs) { return lhs.string() != rhs; }
	inline bool operator!=(Path const & lhs, Path const & rhs) { return lhs.string() != rhs.string(); }

	inline bool operator<(String const & lhs, Path const & rhs) { return lhs < rhs.string(); }
	inline bool operator<(cstring lhs, Path const & rhs) { return lhs < rhs.string(); }
	inline bool operator<(Path const & lhs, cstring rhs) { return lhs.string() < rhs; }
	inline bool operator<(Path const & lhs, Path const & rhs) { return lhs.string() < rhs.string(); }

	inline bool operator>(String const & lhs, Path const & rhs) { return lhs > rhs.string(); }
	inline bool operator>(cstring lhs, Path const & rhs) { return lhs > rhs.string(); }
	inline bool operator>(Path const & lhs, cstring rhs) { return lhs.string() > rhs; }
	inline bool operator>(Path const & lhs, Path const & rhs) { return lhs.string() > rhs.string(); }

	inline bool operator<=(String const & lhs, Path const & rhs) { return lhs <= rhs.string(); }
	inline bool operator<=(cstring lhs, Path const & rhs) { return lhs <= rhs.string(); }
	inline bool operator<=(Path const & lhs, cstring rhs) { return lhs.string() <= rhs; }
	inline bool operator<=(Path const & lhs, Path const & rhs) { return lhs.string() <= rhs.string(); }

	inline bool operator>=(String const & lhs, Path const & rhs) { return lhs >= rhs.string(); }
	inline bool operator>=(cstring lhs, Path const & rhs) { return lhs >= rhs.string(); }
	inline bool operator>=(Path const & lhs, cstring rhs) { return lhs.string() >= rhs; }
	inline bool operator>=(Path const & lhs, Path const & rhs) { return lhs.string() >= rhs.string(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_PATH_HPP_
