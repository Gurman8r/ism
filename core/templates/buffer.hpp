#ifndef _ISM_BUFFER_HPP_
#define _ISM_BUFFER_HPP_

#include <core/templates/array.hpp>
#include <core/templates/vector.hpp>
#include <core/string/string.hpp>
#include <core/templates/mpl.hpp>

#ifndef DYNAMICBUFFER_PRINTV_STRING_SIZE
#define DYNAMICBUFFER_PRINTV_STRING_SIZE 1024
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

		DynamicBuffer(size_t const size_in_bytes, byte const value = byte::null, allocator_type alloc = {}) : m_data{ alloc } { resize(size_in_bytes, value); }

		DynamicBuffer(void const * src, size_t const size_in_bytes, allocator_type alloc = {}) : m_data{ alloc } { write(src, size_in_bytes); }

		template <class Iter = iterator
		> DynamicBuffer(Iter first, Iter last, allocator_type alloc = {}) : m_data{ alloc } { write(first, std::distance(first, last) / sizeof(*first)); }

		DynamicBuffer(storage_type const & data, allocator_type alloc = {}) : m_data{ data, alloc } {}

		DynamicBuffer(storage_type && data, allocator_type alloc = {}) noexcept : m_data{ std::move(data), alloc } {}

		DynamicBuffer(self_type const & value, allocator_type alloc = {}) : m_data{ alloc } { copy(value); }

		DynamicBuffer(self_type && value, allocator_type alloc = {}) noexcept : m_data{ alloc } { swap(value); }

		self_type & operator=(self_type const & value) { return copy(value); }

		self_type & operator=(self_type && value) noexcept { return swap(value); }

	public:
		self_type & clear() noexcept
		{
			return m_data.clear(), (*this);
		}

		self_type & copy(storage_type const & value)
		{
			return (m_data = value), (*this);
		}

		self_type & copy(self_type const & value)
		{
			if (this != std::addressof(value)) { copy(value.m_data); }
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

		self_type & swap(storage_type & value) noexcept
		{
			return m_data.swap(value), (*this);
		}

		self_type & swap(self_type & value) noexcept
		{
			if (this != std::addressof(value)) { swap(value.m_data); }
			return (*this);
		}

	public:
		bool empty() const noexcept { return m_data.empty(); }

		auto capacity() const noexcept -> size_t { return m_data.capacity(); }

		auto size() const noexcept -> size_t { return m_data.size(); }

		auto max_size() const noexcept -> size_t { return m_data.max_size(); }

		auto data() noexcept -> pointer { return m_data.data(); }

		auto data() const noexcept -> const_pointer { return m_data.data(); }

		auto c_str() const noexcept -> cstring { return (cstring)data(); }

		auto begin() noexcept -> iterator { return data(); }

		auto begin() const noexcept -> const_iterator { return data(); }

		auto end() noexcept -> iterator { return begin() + size(); }

		auto end() const noexcept -> const_iterator { return begin() + size(); }

		auto front() noexcept -> reference { ASSERT(!empty()); return *(begin()); }

		auto front() const noexcept -> const_reference { ASSERT(!empty()); return *(begin()); }

		auto back() noexcept -> reference { ASSERT(!empty()); return *(end() - 1); }

		auto back() const noexcept -> const_reference { ASSERT(!empty()); return *(end() - 1); }

		auto operator[](size_t const i) noexcept -> reference { ASSERT(i < size()); return m_data[i]; }

		auto operator[](size_t const i) const noexcept -> byte { ASSERT(i < size()); return m_data[i]; }

		operator void * () noexcept { return m_data.data(); }

		operator void const * () const noexcept { return m_data.data(); }

		operator String() const noexcept { return { c_str(), size() }; }

		operator storage_type & () & noexcept { return m_data; }

		operator storage_type const & () const & noexcept { return m_data; }

		operator storage_type && () && noexcept { return std::move(m_data); }

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
		> T get(size_t const index = 0) const
		{
			T temp;
			return get_to<T>(index, temp);
		}

	public:
		void do_write(size_t const index, void const * src, size_t const size_in_bytes, size_t const align = 0)
		{
			ASSERT(src && "INVALID SOURCE");
			ASSERT(0 < size_in_bytes && "SIZE MUST BE GREATER THAN ZERO");
			size_t const n{ (0 < align && align % 2 == 0) ? SIZE_ROUND_UP(size_in_bytes, align) : size_in_bytes };
			if (size() < index + n) { resize(index + n); }
			copymem(data() + index, src, n);
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
		self_type & printv(size_t const index, cstring fmt, va_list args)
		{
			char s[DYNAMICBUFFER_PRINTV_STRING_SIZE]{};
			if (int32_t const n{ std::vsnprintf(s, sizeof(s), fmt, args) }
			; 0 < n) { do_write(index, s, (size_t)n); }
			return (*this);
		}

		self_type & printv(cstring fmt, va_list args)
		{
			return printv(size(), fmt, args);
		}

		self_type & printf(size_t const index, cstring fmt, ...)
		{
			va_list args;
			va_start(args, fmt);
			printv(index, fmt, args);
			va_end(args);
			return (*this);
		}

		self_type & printf(cstring fmt, ...)
		{
			va_list args;
			va_start(args, fmt);
			printv(size(), fmt, args);
			va_end(args);
			return (*this);
		}

		self_type & print(size_t const index, cstring str, size_t const size_in_bytes) noexcept
		{
			return write(index, str, size_in_bytes);
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

		self_type & print(size_t const index, self_type const & str) noexcept
		{
			return print(index, str.c_str(), str.size());
		}

		self_type & print(self_type const & str) noexcept
		{
			return print(size(), str.c_str(), str.size());
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
		using storage_type		= typename decltype(m_data);
		using reference			= typename byte &;
		using const_reference	= typename byte const &;
		using pointer			= typename byte *;
		using const_pointer		= typename byte const *;
		using iterator			= typename pointer;
		using const_iterator	= typename const_pointer;

		StaticBuffer() noexcept {}

		StaticBuffer(size_t const index, void const * src, size_t const size_in_bytes) noexcept { write(index, src, size_in_bytes); }

		StaticBuffer(void const * src, size_t const size_in_bytes) noexcept { write(src, size_in_bytes); }

		template <class Iter = iterator
		> StaticBuffer(Iter first, Iter last) { write(first, std::distance(first, last) / sizeof(*first)); }

		StaticBuffer(storage_type const & value) { copy(value); }

		StaticBuffer(storage_type && value) noexcept { swap(value); }

		StaticBuffer(self_type const & value) { copy(value); }

		StaticBuffer(self_type && value) noexcept { swap(value); }
	
		self_type & operator=(storage_type const & value) { return copy(value); }
	
		self_type & operator=(storage_type && value) noexcept { return swap(value); }
	
		self_type & operator=(self_type const & value) { return copy(value); }
	
		self_type & operator=(self_type && value) noexcept { return swap(value); }

	public:
		self_type & clear() noexcept
		{
			zeromem(m_data, _Size);
			return (*this);
		}

		self_type & copy(storage_type const & value)
		{
			copymem(m_data, value, _Size);
			return (*this);
		}

		self_type & copy(self_type const & value)
		{
			if (this != std::addressof(value)) { copy(value.m_data); }
			return (*this);
		}

		self_type & swap(storage_type & value) noexcept
		{
			storage_type temp;
			copymem(temp, m_data, _Size);
			copymem(m_data, value, _Size);
			copymem(value, temp, _Size);
			return (*this);
		}

		self_type & swap(self_type & value) noexcept
		{
			if (this != std::addressof(value)) { swap(value.m_data); }
			return (*this);
		}

	public:
		static constexpr bool empty() noexcept { return false; }

		static constexpr auto capacity() noexcept -> size_t { return _Size; }

		static constexpr auto size() noexcept -> size_t { return _Size; }

		static constexpr auto max_size() noexcept -> size_t { return _Size; }

		auto data() noexcept -> pointer { return m_data; }

		auto data() const noexcept -> const_pointer { return m_data; }

		auto c_str() const noexcept -> cstring { return (cstring)m_data; }

		auto begin() noexcept -> iterator { return m_data; }

		auto begin() const noexcept -> const_iterator { return m_data; }

		auto end() noexcept -> iterator { return m_data + _Size; }

		auto end() const noexcept -> const_iterator { return m_data + _Size; }

		auto front() noexcept -> reference { return *m_data; }

		auto front() const noexcept -> const_reference { return *m_data; }

		auto back() noexcept -> reference { return *(m_data + _Size - 1); }

		auto back() const noexcept -> const_reference { return *(m_data + _Size - 1); }

		auto operator[](size_t const i) noexcept -> reference { ASSERT(i < _Size); return m_data[i]; }

		auto operator[](size_t const i) const noexcept -> byte { ASSERT(i < _Size); return m_data[i]; }

		operator void * () noexcept { return m_data; }

		operator void const * () const noexcept { return m_data; }

		operator DynamicBuffer() const noexcept { return { m_data, _Size }; }

		operator String() const noexcept { return { (cstring)m_data, _Size }; }

	public:
		void do_read(size_t const index, void * dst, size_t const size_in_bytes) const
		{
			ASSERT(dst && "INVALID DESTINATION");
			ASSERT(0 < size_in_bytes && "SIZE MUST BE GREATER THAN ZERO");
			ASSERT(index + size_in_bytes <= _Size && "OPERATION WOULD HAVE CAUSED AN OVERFLOW");
			copymem(dst, m_data + index, size_in_bytes);
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
		> T get(size_t const index = 0) const
		{
			T temp;
			return get_to<T>(index, temp);
		}

	public:
		void do_write(size_t const index, void const * src, size_t const size_in_bytes, size_t const align = 0)
		{
			ASSERT(src && "INVALID SOURCE");
			ASSERT(0 < size_in_bytes && "SIZE MUST BE GREATER THAN ZERO");
			size_t const n{ (0 < align && align % 2 == 0) ? SIZE_ROUND_UP(size_in_bytes, align) : size_in_bytes };
			ASSERT(index + n <= _Size && "OPERATION WOULD HAVE CAUSED AN OVERFLOW");
			copymem(m_data + index, src, n);
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
		self_type & printv(size_t const index, cstring fmt, va_list args)
		{
			return std::vsnprintf((char *)m_data + index, _Size - index, fmt, args), (*this);
		}

		self_type & printv(cstring fmt, va_list args)
		{
			return printv(_Size, fmt, args);
		}

		self_type & printf(size_t const index, cstring fmt, ...)
		{
			va_list args;
			va_start(args, fmt);
			printv(index, fmt, args);
			va_end(args);
			return (*this);
		}

		self_type & printf(cstring fmt, ...)
		{
			va_list args;
			va_start(args, fmt);
			printv(_Size, fmt, args);
			va_end(args);
			return (*this);
		}

		self_type & print(size_t const index, cstring str, size_t const size_in_bytes) noexcept
		{
			return write(index, str, size_in_bytes);
		}

		self_type & print(size_t const index, cstring str) noexcept
		{
			return print(index, str, std::strlen(str));
		}

		self_type & print(cstring str, size_t const size_in_bytes) noexcept
		{
			return print(_Size, str, size_in_bytes);
		}

		self_type & print(cstring str) noexcept
		{
			return print(_Size, str, std::strlen(str));
		}

		self_type & print(size_t const index, self_type const & str) noexcept
		{
			return print(index, str.c_str(), str.size());
		}

		self_type & print(self_type const & str) noexcept
		{
			return print(size(), str.c_str(), str.size());
		}

		self_type & print(size_t const index, String const & str) noexcept
		{
			return print(index, str.m_data, str._Size);
		}

		self_type & print(String const & str) noexcept
		{
			return print(_Size, str.m_data, str._Size);
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

		ConstantBuffer(size_t const index, void const * src, size_t const size_in_bytes) noexcept { write(index, src, size_in_bytes); }

		ConstantBuffer(void const * src, size_t const size_in_bytes) noexcept { write(src, size_in_bytes); }

		template <class Iter = iterator
		> ConstantBuffer(Iter first, Iter last) { write(first, std::distance(first, last) / sizeof(*first)); }

		ConstantBuffer(storage_type const & value) { copy(value); }

		ConstantBuffer(storage_type && value) noexcept { swap(value); }

		ConstantBuffer(self_type const & value) { copy(value); }

		ConstantBuffer(self_type && value) noexcept { swap(value); }

		self_type & operator=(storage_type const & value) { return copy(value); }

		self_type & operator=(storage_type && value) noexcept { return swap(value); }

		self_type & operator=(self_type const & value) { return copy(value); }

		self_type & operator=(self_type && value) noexcept { return swap(value); }

	public:
		self_type & clear() noexcept
		{
			zeromem(m_data, _Size);
			return (*this);
		}

		self_type & copy(storage_type const & value)
		{
			copymem(m_data, value, _Size);
			return (*this);
		}

		self_type & copy(self_type const & value)
		{
			if (this != std::addressof(value)) { copy(value.m_data); }
			return (*this);
		}

		self_type & swap(storage_type & value) noexcept
		{
			storage_type temp;
			copymem(temp, m_data, _Size);
			copymem(m_data, value, _Size);
			copymem(value, temp, _Size);
			return (*this);
		}

		self_type & swap(self_type & value) noexcept
		{
			if (this != std::addressof(value)) { swap(value.m_data); }
			return (*this);
		}

	public:
		static constexpr bool empty() noexcept { return false; }

		static constexpr auto capacity() noexcept -> size_t { return _Size; }

		static constexpr auto size() noexcept -> size_t { return _Size; }

		static constexpr auto max_size() noexcept -> size_t { return _Size; }

		auto data() noexcept -> pointer { return m_data; }

		auto data() const noexcept -> const_pointer { return m_data; }

		auto c_str() const noexcept -> cstring { return (cstring)m_data; }

		auto begin() noexcept -> iterator { return m_data; }

		auto begin() const noexcept -> const_iterator { return m_data; }

		auto end() noexcept -> iterator { return m_data + _Size; }

		auto end() const noexcept -> const_iterator { return m_data + _Size; }

		auto front() noexcept -> reference { return *m_data; }

		auto front() const noexcept -> const_reference { return *m_data; }

		auto back() noexcept -> reference { return *(m_data + _Size - 1); }

		auto back() const noexcept -> const_reference { return *(m_data + _Size - 1); }

		auto operator[](size_t const i) noexcept -> reference { ASSERT(i < _Size); return m_data[i]; }

		auto operator[](size_t const i) const noexcept -> byte { ASSERT(i < _Size); return m_data[i]; }

		operator void * () noexcept { return m_data; }

		operator void const * () const noexcept { return m_data; }

		operator DynamicBuffer() const noexcept { return { m_data, _Size }; }

		operator String() const noexcept { return { (cstring)m_data, _Size }; }

	public:
		void do_read(size_t const index, void * dst, size_t const size_in_bytes) const
		{
			ASSERT(dst && "INVALID DESTINATION");
			ASSERT(0 < size_in_bytes && "SIZE MUST BE GREATER THAN ZERO");
			ASSERT(index + size_in_bytes <= _Size && "OPERATION WOULD HAVE CAUSED AN OVERFLOW");
			copymem(dst, m_data + index, size_in_bytes);
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
		> T get(size_t const index = 0) const
		{
			T temp;
			return get_to<T>(index, temp);
		}

		template <size_t I
		> type_i<I> get() const
		{
			return get<type_i<I>>(_calc_index<I>());
		}

	public:
		void do_write(size_t const index, void const * src, size_t const size_in_bytes)
		{
			ASSERT(src && "INVALID SOURCE");
			ASSERT(0 < size_in_bytes && "SIZE MUST BE GREATER THAN ZERO");
			ASSERT(index + size_in_bytes <= _Size && "OPERATION WOULD HAVE CAUSED AN OVERFLOW");
			copymem(m_data + index, src, size_in_bytes);
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
		self_type & printv(size_t const index, cstring fmt, va_list args)
		{
			return std::vsnprintf((char *)m_data + index, _Size - index, fmt, args), (*this);
		}

		self_type & printv(cstring fmt, va_list args)
		{
			return printv(_Size, fmt, args);
		}

		self_type & printf(size_t const index, cstring fmt, ...)
		{
			va_list args;
			va_start(args, fmt);
			printv(index, fmt, args);
			va_end(args);
			return (*this);
		}

		self_type & printf(cstring fmt, ...)
		{
			va_list args;
			va_start(args, fmt);
			printv(_Size, fmt, args);
			va_end(args);
			return (*this);
		}

		self_type & print(size_t const index, cstring str, size_t const size_in_bytes) noexcept
		{
			return write(index, str, size_in_bytes);
		}

		self_type & print(size_t const index, cstring str) noexcept
		{
			return print(index, str, std::strlen(str));
		}

		self_type & print(cstring str, size_t const size_in_bytes) noexcept
		{
			return print(_Size, str, size_in_bytes);
		}

		self_type & print(cstring str) noexcept
		{
			return print(_Size, str, std::strlen(str));
		}

		self_type & print(size_t const index, self_type const & str) noexcept
		{
			return print(index, str.c_str(), str.size());
		}

		self_type & print(self_type const & str) noexcept
		{
			return print(size(), str.c_str(), str.size());
		}

		self_type & print(size_t const index, String const & str) noexcept
		{
			return print(index, str.m_data, str._Size);
		}

		self_type & print(String const & str) noexcept
		{
			return print(_Size, str.m_data, str._Size);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_BUFFER_HPP_
