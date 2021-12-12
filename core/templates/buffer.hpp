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
		using allocator_type	= typename PolymorphicAllocator<>;
		using reference			= typename byte &;
		using const_reference	= typename byte const &;
		using pointer			= typename byte *;
		using const_pointer		= typename byte const *;
		using iterator			= typename pointer;
		using const_iterator	= typename const_pointer;

		static constexpr byte null{ (byte)'\0' };

		~Buffer() noexcept = default;

		Buffer(allocator_type alloc = {}) noexcept : m_data{ alloc } {}

		Buffer(storage_type const & data, allocator_type alloc = {}) : m_data{ data, alloc } {}

		Buffer(storage_type && data, allocator_type alloc = {}) noexcept : m_data{ std::move(data), alloc } {}

		Buffer(size_t const size_in_bytes, byte const value = null, allocator_type alloc = {}) : m_data{ alloc } { resize(size_in_bytes, value); }

		Buffer(void const * src, size_t const size_in_bytes, allocator_type alloc = {}) : m_data{ alloc } { write(src, size_in_bytes); }

		template <class Iter = iterator
		> Buffer(Iter first, Iter last, allocator_type alloc = {}) : m_data{ alloc } { write(first, std::distance(first, last) / sizeof(*first)); }

		Buffer(Buffer const & other, allocator_type alloc = {}) : m_data{ other.m_data, alloc } {}

		Buffer(Buffer && other, allocator_type alloc = {}) noexcept : m_data{ alloc } { swap(std::move(other)); }

		Buffer & operator=(Buffer const & other) { Buffer temp{ other }; return swap(temp); }

		Buffer & operator=(Buffer && other) noexcept { return swap(std::move(other)); }

		NODISCARD operator storage_type & () & noexcept { return m_data; }

		NODISCARD operator storage_type const & () const & noexcept { return m_data; }

		NODISCARD operator storage_type && () && noexcept { return std::move(m_data); }

	public:
		NODISCARD bool empty() const noexcept { return m_data.empty(); }

		NODISCARD auto size() const noexcept -> size_t { return m_data.size(); }

		NODISCARD auto data() noexcept -> pointer { return m_data.data(); }

		NODISCARD auto data() const noexcept -> const_pointer { return m_data.data(); }

		NODISCARD auto c_str() const noexcept -> cstring { return (cstring)data(); }

		NODISCARD auto begin() noexcept -> iterator { return data(); }

		NODISCARD auto begin() const noexcept -> const_iterator { return data(); }

		NODISCARD auto end() noexcept -> iterator { return begin() + size(); }

		NODISCARD auto end() const noexcept -> const_iterator { return begin() + size(); }

		NODISCARD auto front() noexcept -> reference { ASSERT(!empty()); return *(begin()); }

		NODISCARD auto front() const noexcept -> const_reference { ASSERT(!empty()); return *(begin()); }

		NODISCARD auto back() noexcept -> reference { ASSERT(!empty()); return *(end() - 1); }

		NODISCARD auto back() const noexcept -> const_reference { ASSERT(!empty()); return *(end() - 1); }

		NODISCARD auto operator[](size_t const i) noexcept -> reference { ASSERT(i < size()); return m_data[i]; }

		NODISCARD auto operator[](size_t const i) const noexcept -> byte { ASSERT(i < size()); return m_data[i]; }

	public:
		Buffer & clear() noexcept
		{
			return m_data.clear(), (*this);
		}

		Buffer & reserve(size_t const size_in_bytes)
		{
			if (size() < size_in_bytes)
			{
				resize(size_in_bytes, null);
			}
			return (*this);
		}

		Buffer & resize(size_t const size_in_bytes, byte const value = null)
		{
			return m_data.resize(size_in_bytes, value), (*this);
		}

		Buffer & swap(Buffer & other) noexcept
		{
			if (this != std::addressof(other))
			{
				m_data.swap(other.m_data);
			}
			return (*this);
		}

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
			return get_to<T>(0, value);
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
			if constexpr (1 == sizeof(T))
			{
				return m_data.push_back(static_cast<byte>(value)), (*this);
			}
			else
			{
				return write(value);
			}
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
