#ifndef _ISM_BUFFER_HPP_
#define _ISM_BUFFER_HPP_

#include <core/error/error_macros.hpp>
#include <core/templates/array.hpp>
#include <core/templates/vector.hpp>
#include <core/string/basic_string.hpp>

#ifndef DYNAMICBUFFER_VPRINTF_STRING_SIZE
#define DYNAMICBUFFER_VPRINTF_STRING_SIZE 1024
#endif

// dynamic buffer
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class DynamicBuffer
	{
		Vector<byte> m_data{};

	public:
		using self_type			= typename DynamicBuffer;
		using storage_type		= typename decltype(m_data);
		using allocator_type	= typename PolymorphicAllocator<>;
		using reference			= typename byte &;
		using const_reference	= typename byte const &;
		using pointer			= typename byte *;
		using const_pointer		= typename byte const *;
		using iterator			= typename pointer;
		using const_iterator	= typename const_pointer;

		DynamicBuffer(allocator_type alloc = {}) noexcept : m_data{ alloc } {}

		DynamicBuffer(storage_type const & data, allocator_type alloc = {}) : m_data{ data, alloc } {}

		DynamicBuffer(storage_type && data, allocator_type alloc = {}) noexcept : m_data{ std::move(data), alloc } {}

		DynamicBuffer(size_t const size_in_bytes, byte const value = byte::null, allocator_type alloc = {}) : m_data{ alloc } { resize(size_in_bytes, value); }

		DynamicBuffer(void const * src, size_t const size_in_bytes, allocator_type alloc = {}) : m_data{ alloc } { write(src, size_in_bytes); }

		template <class Iter = iterator
		> DynamicBuffer(Iter first, Iter last, allocator_type alloc = {}) : m_data{ alloc } { write(first, std::distance(first, last) / sizeof(*first)); }

		DynamicBuffer(self_type const & other, allocator_type alloc = {}) : m_data{ other.m_data, alloc } {}

		self_type & operator=(self_type const & other) { self_type temp{ other }; return swap(temp); }

	public:
		NODISCARD bool empty() const noexcept { return m_data.empty(); }

		NODISCARD auto capacity() const noexcept -> size_t { return m_data.capacity(); }

		NODISCARD auto size() const noexcept -> size_t { return m_data.size(); }

		NODISCARD auto max_size() const noexcept -> size_t { return m_data.max_size(); }

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

		NODISCARD operator void * () noexcept { return m_data.data(); }

		NODISCARD operator void const * () const noexcept { return m_data.data(); }

		NODISCARD operator String() const noexcept { return { c_str(), size() }; }

		NODISCARD operator storage_type & () & noexcept { return m_data; }

		NODISCARD operator storage_type const & () const & noexcept { return m_data; }

		NODISCARD operator storage_type && () && noexcept { return std::move(m_data); }

	public:
		self_type & clear() noexcept
		{
			return m_data.clear(), (*this);
		}

		self_type & copy(self_type const & other)
		{
			if (this != std::addressof(other)) { m_data = other.m_data; }
			return (*this);
		}

		self_type & resize(size_t const size_in_bytes, byte const value = byte::null)
		{
			return m_data.resize(size_in_bytes, value), (*this);
		}

		self_type & shrink_to_fit()
		{
			return m_data.shrink_to_fit(), (*this);
		}

		self_type & swap(self_type & other) noexcept
		{
			if (this != std::addressof(other)) { m_data.swap(other.m_data); }
			return (*this);
		}

	public:
		void do_read(size_t const index, void * dst, size_t const size_in_bytes) const
		{
			ASSERT(dst && "INVALID DESTINATION");
			ASSERT(0 < size_in_bytes && "SIZE MUST BE GREATER THAN ZERO");
			ASSERT(index + size_in_bytes < size() && "OPERATION WOULD CAUSE AN OVERFLOW");
			copymem(dst, data() + index, size_in_bytes);
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> T & get_to(size_t const index, T & value) const
		{
			do_read(index, std::addressof(value), sizeof(T));
			return value;
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> T & get_to(T & value) const
		{
			return get_to<T>(0, value);
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> NODISCARD T get(size_t const index = 0) const
		{
			T temp;
			return get_to<T>(index, temp);
		}

	public:
		void do_write(size_t const index, void const * src, size_t const size_in_bytes, size_t const alignment = 0)
		{
			ASSERT(src && "INVALID SOURCE");
			ASSERT(0 < size_in_bytes && "SIZE MUST BE GREATER THAN ZERO");
			if (size() < index + size_in_bytes) { resize(index + size_in_bytes); }
			copymem(data() + index, src, size_in_bytes);
		}

		self_type & write(size_t const index, void const * src, size_t const size_in_bytes)
		{
			if (src && 0 < size_in_bytes) { do_write(index, src, size_in_bytes); }
			return (*this);
		}

		self_type & write(void const * src, size_t const size_in_bytes)
		{
			if (src && 0 < size_in_bytes) { do_write(size(), src, size_in_bytes); }
			return (*this);
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> self_type & write(size_t const index, T const & value)
		{
			do_write(index, std::addressof(value), sizeof(T));
			return (*this);
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> self_type & write(T const & value)
		{
			do_write(size(), std::addressof(value), sizeof(T));
			return (*this);
		}

	public:
		self_type & vprintf(size_t const index, cstring fmt, va_list args)
		{
			char str[DYNAMICBUFFER_VPRINTF_STRING_SIZE]{};
			if (int32_t const len{ std::vsnprintf(str, sizeof(str), fmt, args) }
			; 0 < len) { do_write(index, str, (size_t)len); }
			return (*this);
		}

		self_type & vprintf(cstring fmt, va_list args)
		{
			return vprintf(size(), fmt, args);
		}

		self_type & printf(size_t const index, cstring fmt, ...)
		{
			va_list args;
			va_start(args, fmt);
			vprintf(index, fmt, args);
			va_end(args);
			return (*this);
		}

		self_type & printf(cstring fmt, ...)
		{
			va_list args;
			va_start(args, fmt);
			vprintf(size(), fmt, args);
			va_end(args);
			return (*this);
		}

		self_type & print(size_t const index, cstring str, size_t const size_in_bytes) noexcept
		{
			if (str && *str && size_in_bytes) { do_write(index, str, size_in_bytes); }
			return (*this);
		}

		self_type & print(size_t const index, cstring str) noexcept
		{
			return print(index, str, std::strlen(str));
		}

		self_type & print(cstring str, size_t const size_in_bytes) noexcept
		{
			return print(size(), str, size_in_bytes);
		}

		self_type & print(cstring str) noexcept
		{
			return print(size(), str, std::strlen(str));
		}

		self_type & print(size_t const index, String const & str) noexcept
		{
			return print(index, str.data(), str.size());
		}

		self_type & print(String const & str) noexcept
		{
			return print(size(), str.data(), str.size());
		}

	public:
		template <class T
		> self_type & operator<<(T const & value)
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

		self_type & operator<<(cstring str)
		{
			return print(str);
		}

		self_type & operator<<(String const & str)
		{
			return print(str);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// static buffer
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <size_t _Size
	> class StaticBuffer
	{
		static_assert(0 < _Size);

		Array<byte, _Size> m_data{};

	public:
		using self_type			= typename StaticBuffer<_Size>;
		using reference			= typename byte &;
		using const_reference	= typename byte const &;
		using pointer			= typename byte *;
		using const_pointer		= typename byte const *;
		using iterator			= typename pointer;
		using const_iterator	= typename const_pointer;

		StaticBuffer() noexcept {}

		StaticBuffer(self_type const & other) { copy(other); }
	
		StaticBuffer(self_type && other) noexcept { swap(std::move(other)); }

		StaticBuffer(size_t const index, void const * src, size_t const size_in_bytes) noexcept { write(index, src, size_in_bytes); }

		StaticBuffer(void const * src, size_t const size_in_bytes) noexcept { write(0, src, size_in_bytes); }

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> StaticBuffer(size_t const index, T const & value) noexcept { write(index, value); }

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> StaticBuffer(T const & value) noexcept { write(0, value); }
	
		self_type & operator=(self_type const & other) { self_type temp{ other }; return swap(temp); }
	
		self_type & operator=(self_type && other) noexcept { return swap(std::move(other)); }

	public:
		self_type & clear() noexcept
		{
			zeromem(m_data, _Size);
			return (*this);
		}
	
		self_type & copy(self_type const & other)
		{
			if (this != std::addressof(other)) {
				copymem(m_data, other.m_data, _Size);
			}
			return (*this);
		}
	
		self_type & swap(self_type & other) noexcept
		{
			if (this != std::addressof(other)) {
				byte temp[_Size];
				copymem(temp, m_data, _Size);
				copymem(m_data, other.m_data, _Size);
				copymem(other.m_data, temp, _Size);
			}
			return (*this);
		}

	public:
		NODISCARD bool empty() const noexcept { return false; }

		NODISCARD auto capacity() const noexcept -> size_t { return _Size; }

		NODISCARD auto size() const noexcept -> size_t { return _Size; }

		NODISCARD auto max_size() const noexcept -> size_t { return _Size; }

		NODISCARD auto data() noexcept -> pointer { return m_data; }

		NODISCARD auto data() const noexcept -> const_pointer { return m_data; }

		NODISCARD auto c_str() const noexcept -> cstring { return (cstring)m_data; }

		NODISCARD auto begin() noexcept -> iterator { return m_data; }

		NODISCARD auto begin() const noexcept -> const_iterator { return m_data; }

		NODISCARD auto end() noexcept -> iterator { return m_data + _Size; }

		NODISCARD auto end() const noexcept -> const_iterator { return m_data + _Size; }

		NODISCARD auto front() noexcept -> reference { return *m_data; }

		NODISCARD auto front() const noexcept -> const_reference { return *m_data; }

		NODISCARD auto back() noexcept -> reference { return *(m_data + _Size - 1); }

		NODISCARD auto back() const noexcept -> const_reference { return *(m_data + _Size - 1); }

		NODISCARD auto operator[](size_t const i) noexcept -> reference { ASSERT(i < _Size); return m_data[i]; }

		NODISCARD auto operator[](size_t const i) const noexcept -> byte { ASSERT(i < _Size); return m_data[i]; }

		NODISCARD operator void * () noexcept { return m_data; }

		NODISCARD operator void const * () const noexcept { return m_data; }

		NODISCARD operator DynamicBuffer() const noexcept { return { m_data, _Size }; }

		NODISCARD operator String() const noexcept { return { c_str(), _Size }; }

	public:
		void do_read(size_t const index, void * dst, size_t const size_in_bytes) const
		{
			ASSERT(dst && "INVALID DESTINATION");
			ASSERT(0 < size_in_bytes && "SIZE MUST BE GREATER THAN ZERO");
			ASSERT(index + size_in_bytes <= _Size && "OPERATION WOULD HAVE CAUSED AN OVERFLOW");
			copymem(dst, data() + index, size_in_bytes);
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> T & get_to(size_t const index, T & value) const
		{
			do_read(index, std::addressof(value), sizeof(T));
			return value;
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> T & get_to(T & value) const
		{
			return get_to<T>(0, value);
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> NODISCARD T get(size_t const index = 0) const
		{
			T temp;
			return get_to<T>(index, temp);
		}

	public:
		void do_write(size_t const index, void const * src, size_t const size_in_bytes)
		{
			ASSERT(src && "INVALID SOURCE");
			ASSERT(0 < size_in_bytes && "SIZE MUST BE GREATER THAN ZERO");
			ASSERT(index + size_in_bytes <= _Size && "OPERATION WOULD HAVE CAUSED AN OVERFLOW");
			copymem(data() + index, src, size_in_bytes);
		}

		self_type & write(size_t const index, void const * src, size_t const size_in_bytes)
		{
			if (src && 0 < size_in_bytes) { do_write(index, src, size_in_bytes); }
			return (*this);
		}

		self_type & write(void const * src, size_t const size_in_bytes)
		{
			if (src && 0 < size_in_bytes) { do_write(0, src, size_in_bytes); }
			return (*this);
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> self_type & write(size_t const index, T const & value)
		{
			do_write(index, std::addressof(value), sizeof(T));
			return (*this);
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> self_type & write(T const & value)
		{
			do_write(0, std::addressof(value), sizeof(T));
			return (*this);
		}

	public:
		self_type & vprintf(size_t const index, cstring fmt, va_list args)
		{
			std::vsnprintf((char *)data() + index, _Size, fmt, args);
			return (*this);
		}

		self_type & vprintf(cstring fmt, va_list args)
		{
			return vprintf(size(), fmt, args);
		}

		self_type & printf(size_t const index, cstring fmt, ...)
		{
			va_list args;
			va_start(args, fmt);
			vprintf(index, fmt, args);
			va_end(args);
			return (*this);
		}

		self_type & printf(cstring fmt, ...)
		{
			va_list args;
			va_start(args, fmt);
			vprintf(size(), fmt, args);
			va_end(args);
			return (*this);
		}

		self_type & print(size_t const index, cstring str, size_t const size_in_bytes) noexcept
		{
			if (str && *str && size_in_bytes) { do_write(index, str, size_in_bytes); }
			return (*this);
		}

		self_type & print(size_t const index, cstring str) noexcept
		{
			return print(index, str, std::strlen(str));
		}

		self_type & print(cstring str, size_t const size_in_bytes) noexcept
		{
			return print(size(), str, size_in_bytes);
		}

		self_type & print(cstring str) noexcept
		{
			return print(size(), str, std::strlen(str));
		}

		self_type & print(size_t const index, String const & str) noexcept
		{
			return print(index, str.data(), str.size());
		}

		self_type & print(String const & str) noexcept
		{
			return print(size(), str.data(), str.size());
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// constant buffer
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	template <size_t _Alignment, class ... _Types
	> class ConstantBuffer
	{
		static_assert(0 < _Alignment && _Alignment % 2 == 0);
		static_assert(0 < sizeof...(_Types));

		template <size_t I
		> using type_i = mpl::nth<I, mpl::type_list<_Types...>>;

		template <size_t I = 0, size_t Result = 0
		> static constexpr size_t _calc_size() noexcept
		{
			if constexpr (I < sizeof...(_Types))
			{
				return _calc_size<I + 1, Result + SIZE_ROUND_UP(sizeof(type_i<I>), _Alignment)>();
			}
			else
			{
				return Result;
			}
		}

		template <size_t Index, size_t I = 0, size_t Result = 0
		> static constexpr size_t _calc_index() noexcept
		{
			static_assert(Index < sizeof...(_Types));
			if constexpr (I < Index)
			{
				return _calc_index<Index, I + 1, Result + SIZE_ROUND_UP(sizeof(type_i<I>), _Alignment)>();
			}
			else
			{
				return Result;
			}
		}

		static constexpr size_t _Size{ _calc_size() };

		Array<byte, _Size> m_data{};

	public:
		using self_type			= typename ConstantBuffer<_Alignment, _Types...>;
		using storage_type		= typename decltype(m_data);
		using reference			= typename byte &;
		using const_reference	= typename byte const &;
		using pointer			= typename byte *;
		using const_pointer		= typename byte const *;
		using iterator			= typename pointer;
		using const_iterator	= typename const_pointer;

		ConstantBuffer() noexcept {}

		ConstantBuffer(self_type const & other) { copy(other); }

		ConstantBuffer(self_type && other) noexcept { swap(std::move(other)); }

		self_type & operator=(self_type const & other) { self_type temp{ other }; return swap(temp); }

		self_type & operator=(self_type && other) noexcept { return swap(std::move(other)); }

	public:
		self_type & clear() noexcept
		{
			zeromem(m_data, _Size);
			return (*this);
		}

		self_type & copy(self_type const & other)
		{
			if (this != std::addressof(other)) {
				copymem(m_data, other.m_data, _Size);
			}
			return (*this);
		}

		self_type & swap(self_type & other) noexcept
		{
			if (this != std::addressof(other)) {
				byte temp[_Size];
				copymem(temp, m_data, _Size);
				copymem(m_data, other.m_data, _Size);
				copymem(other.m_data, temp, _Size);
			}
			return (*this);
		}

	public:
		NODISCARD bool empty() const noexcept { return false; }

		NODISCARD auto capacity() const noexcept -> size_t { return _Size; }

		NODISCARD auto size() const noexcept -> size_t { return _Size; }

		NODISCARD auto max_size() const noexcept -> size_t { return _Size; }

		NODISCARD auto data() noexcept -> pointer { return m_data; }

		NODISCARD auto data() const noexcept -> const_pointer { return m_data; }

		NODISCARD auto c_str() const noexcept -> cstring { return (cstring)m_data; }

		NODISCARD auto begin() noexcept -> iterator { return m_data; }

		NODISCARD auto begin() const noexcept -> const_iterator { return m_data; }

		NODISCARD auto end() noexcept -> iterator { return m_data + _Size; }

		NODISCARD auto end() const noexcept -> const_iterator { return m_data + _Size; }

		NODISCARD auto front() noexcept -> reference { return *m_data; }

		NODISCARD auto front() const noexcept -> const_reference { return *m_data; }

		NODISCARD auto back() noexcept -> reference { return *(m_data + _Size - 1); }

		NODISCARD auto back() const noexcept -> const_reference { return *(m_data + _Size - 1); }

		NODISCARD auto operator[](size_t const i) noexcept -> reference { ASSERT(i < _Size); return m_data[i]; }

		NODISCARD auto operator[](size_t const i) const noexcept -> byte { ASSERT(i < _Size); return m_data[i]; }

		NODISCARD operator void * () noexcept { return m_data; }

		NODISCARD operator void const * () const noexcept { return m_data; }

		NODISCARD operator DynamicBuffer() const noexcept { return { m_data, _Size }; }

		NODISCARD operator String() const noexcept { return { c_str(), _Size }; }

	public:
		void do_read(size_t const index, void * dst, size_t const size_in_bytes) const
		{
			ASSERT(dst && "INVALID DESTINATION");
			ASSERT(0 < size_in_bytes && "SIZE MUST BE GREATER THAN ZERO");
			ASSERT(index + size_in_bytes <= _Size && "OPERATION WOULD HAVE CAUSED AN OVERFLOW");
			copymem(dst, data() + index, size_in_bytes);
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> T & get_to(size_t const index, T & value) const
		{
			do_read(index, std::addressof(value), sizeof(T));
			return value;
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> T & get_to(T & value) const
		{
			return get_to<T>(0, value);
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> NODISCARD T get(size_t const index = 0) const
		{
			T temp;
			return get_to<T>(index, temp);
		}

		template <size_t I
		> NODISCARD type_i<I> get() const
		{
			return get<type_i<I>>(_calc_index<I>());
		}

	public:
		void do_write(size_t const index, void const * src, size_t const size_in_bytes)
		{
			ASSERT(src && "INVALID SOURCE");
			ASSERT(0 < size_in_bytes && "SIZE MUST BE GREATER THAN ZERO");
			ASSERT(index + size_in_bytes <= _Size && "OPERATION WOULD HAVE CAUSED AN OVERFLOW");
			copymem(data() + index, src, size_in_bytes);
		}

		self_type & write(size_t const index, void const * src, size_t const size_in_bytes)
		{
			if (src && 0 < size_in_bytes) { do_write(index, src, size_in_bytes); }
			return (*this);
		}

		self_type & write(void const * src, size_t const size_in_bytes)
		{
			if (src && 0 < size_in_bytes) { do_write(0, src, size_in_bytes); }
			return (*this);
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> self_type & write(size_t const index, T const & value)
		{
			return do_write(index, std::addressof(value), sizeof(T)), (*this);
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> self_type & write(T const & value)
		{
			return do_write(0, std::addressof(value), sizeof(T)), (*this);
		}

		template <size_t I
		> self_type & set(type_i<I> const & value)
		{
			return write(_calc_index<I>(), value);
		}

	public:
		self_type & vprintf(size_t const index, cstring fmt, va_list args)
		{
			std::vsnprintf((char *)data() + index, _Size, fmt, args);
			return (*this);
		}

		self_type & vprintf(cstring fmt, va_list args)
		{
			return vprintf(size(), fmt, args);
		}

		self_type & printf(size_t const index, cstring fmt, ...)
		{
			va_list args;
			va_start(args, fmt);
			vprintf(index, fmt, args);
			va_end(args);
			return (*this);
		}

		self_type & printf(cstring fmt, ...)
		{
			va_list args;
			va_start(args, fmt);
			vprintf(size(), fmt, args);
			va_end(args);
			return (*this);
		}

		self_type & print(size_t const index, cstring str, size_t const size_in_bytes) noexcept
		{
			if (str && *str && size_in_bytes) { do_write(index, str, size_in_bytes); }
			return (*this);
		}

		self_type & print(size_t const index, cstring str) noexcept
		{
			return print(index, str, std::strlen(str));
		}

		self_type & print(cstring str, size_t const size_in_bytes) noexcept
		{
			return print(size(), str, size_in_bytes);
		}

		self_type & print(cstring str) noexcept
		{
			return print(size(), str, std::strlen(str));
		}

		self_type & print(size_t const index, String const & str) noexcept
		{
			return print(index, str.data(), str.size());
		}

		self_type & print(String const & str) noexcept
		{
			return print(size(), str.data(), str.size());
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_BUFFER_HPP_
