#ifndef _ISM_BUFFER_HPP_
#define _ISM_BUFFER_HPP_

#include <core/error/error_macros.hpp>
#include <core/templates/vector.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Buffer
	{
		Vector<byte> m_data{};

	public:
		using storage_type		= typename decltype(m_data);
		using pointer			= typename byte *;
		using const_pointer		= typename byte const *;
		using iterator			= typename pointer;
		using const_iterator	= typename const_pointer;

		static constexpr byte null{ (byte)'\0' };

		~Buffer() noexcept = default;

		Buffer() noexcept {}

		explicit Buffer(storage_type const & data) : m_data{ data } {}

		explicit Buffer(storage_type && data) noexcept : m_data{ std::move(data) } {}

		explicit Buffer(size_t const size_in_bytes, byte const value = null) { resize(size_in_bytes, value); }

		Buffer(Buffer const & other) : m_data{ other.m_data } {}

		Buffer(Buffer && other) noexcept { swap(std::move(other)); }

		Buffer & operator=(Buffer const & other) { Buffer temp{ other }; return swap(temp); }

		Buffer & operator=(Buffer && other) noexcept { return swap(std::move(other)); }

		Buffer & swap(Buffer & other) noexcept { if (this != std::addressof(other)) { m_data.swap(other.m_data); } return (*this); }

		NODISCARD operator storage_type const & () const & noexcept { return m_data; }

		NODISCARD operator storage_type && () && noexcept { return std::move(m_data); }

	public:
		NODISCARD bool empty() const noexcept { return m_data.empty(); }

		NODISCARD auto size() const noexcept -> size_t { return m_data.size(); }

		NODISCARD auto data() noexcept -> byte * { return m_data.data(); }

		NODISCARD auto data() const noexcept -> byte const * { return m_data.data(); }

		NODISCARD auto c_str() const noexcept -> cstring { return (cstring)data(); }

		NODISCARD auto begin() noexcept -> iterator { return data(); }

		NODISCARD auto begin() const noexcept -> const_iterator { return data(); }

		NODISCARD auto end() noexcept -> iterator { return begin() + size(); }

		NODISCARD auto end() const noexcept -> const_iterator { return begin() + size(); }

		NODISCARD auto front() noexcept -> byte & { ASSERT(!empty()); return *(begin()); }

		NODISCARD auto front() const noexcept -> byte const & { ASSERT(!empty()); return *(begin()); }

		NODISCARD auto back() noexcept -> byte & { ASSERT(!empty()); return *(end() - 1); }

		NODISCARD auto back() const noexcept -> byte const & { ASSERT(!empty()); return *(end() - 1); }

		NODISCARD auto operator[](size_t const i) noexcept -> byte & { ASSERT(i < size()); return m_data[i]; }

		NODISCARD auto operator[](size_t const i) const noexcept -> byte { ASSERT(i < size()); return m_data[i]; }

	public:
		void clear() noexcept { m_data.clear(); }

		void reserve(size_t const size_in_bytes) { if (size() < size_in_bytes) { m_data.resize(size_in_bytes, null); } }

		void resize(size_t const size_in_bytes, byte const value = null) { m_data.resize(size_in_bytes, value); }

	public:
		void * get_to(size_t const index, void * dst, size_t const size_in_bytes)
		{
			ASSERT(index + size_in_bytes <= size());
			copymem(dst, begin() + index, size_in_bytes);
			return dst;
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> T & get_to(size_t const index, T & value)
		{
			get_to(index, std::addressof(value), sizeof(T));
			return value;
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> T & get_to(T & value)
		{
			return get_to(0, value);
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> NODISCARD T get(size_t const index = 0)
		{
			T temp;
			return get_to<T>(index, temp);
		}

	protected:
		void do_write(size_t const index, void const * src, size_t const size_in_bytes)
		{
			reserve(index + size_in_bytes);

			copymem(begin() + index, src, size_in_bytes);
		}

	public:
		Buffer & write(size_t const index, void const * src, size_t const size_in_bytes)
		{
			if (src && size_in_bytes) { do_write(index, src, size_in_bytes); }
			return (*this);
		}

		Buffer & write(void const * src, size_t const size_in_bytes)
		{
			if (src && size_in_bytes) { do_write(size(), src, size_in_bytes); }
			return (*this);
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> Buffer & write(size_t const index, T const & value)
		{
			do_write(index, std::addressof(value), sizeof(T));
			return (*this);
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> Buffer & write(T const & value)
		{
			do_write(size(), std::addressof(value), sizeof(T));
			return (*this);
		}

	public:
		Buffer & vprintf(size_t const index, cstring fmt, va_list args)
		{
			if (int32_t size_in_bytes{ std::vsnprintf(nullptr, 0, fmt, args) }; 0 < size_in_bytes)
			{
				size_in_bytes++; // account for null teminator

				reserve(index + (size_t)size_in_bytes);

				std::vsprintf((char *)begin() + index, fmt, args);
			}
			return (*this);
		}

		Buffer & vprintf(cstring fmt, va_list args)
		{
			return vprintf(size(), fmt, args);
		}

		Buffer & printf(size_t const index, cstring fmt, ...)
		{
			if (!fmt) { return (*this); }
			va_list args;
			va_start(args, fmt);
			vprintf(index, fmt, args);
			va_end(args);
			return (*this);
		}

		Buffer & printf(cstring fmt, ...)
		{
			if (!fmt) { return (*this); }
			va_list args;
			va_start(args, fmt);
			vprintf(size(), fmt, args);
			va_end(args);
			return (*this);
		}

		Buffer & print(size_t const index, String const & str)
		{
			if (!str.empty()) { do_write(index, str.data(), str.size()); }
			return (*this);
		}

		Buffer & print(String const & str)
		{
			if (!str.empty()) { do_write(size(), str.data(), str.size()); }
			return (*this);
		}

		Buffer & print(size_t const index, cstring str, size_t const size_in_bytes)
		{
			if (str && size_in_bytes) { do_write(index, str, size_in_bytes); }
			return (*this);
		}

		Buffer & print(size_t const index, cstring str)
		{
			if (str) { do_write(index, str, std::strlen(str)); }
			return (*this);
		}

		Buffer & print(cstring str, size_t const size_in_bytes)
		{
			if (str && size_in_bytes) { do_write(size(), str, size_in_bytes); }
			return (*this);
		}

		Buffer & print(cstring str)
		{
			if (str) { do_write(size(), str, std::strlen(str)); }
			return (*this);
		}

	public:
		template <class T
		> Buffer & operator<<(T const & value)
		{
			return write(value);
		}
		
		Buffer & operator<<(cstring str)
		{
			return print(str);
		}

		Buffer & operator<<(String const & str)
		{
			return print(str);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_BUFFER_HPP_
