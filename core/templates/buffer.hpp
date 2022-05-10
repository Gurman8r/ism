#ifndef _ISM_BUFFER_HPP_
#define _ISM_BUFFER_HPP_

#include <core/error/error_macros.hpp>
#include <core/templates/vector.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class DynamicBuffer
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

		~DynamicBuffer() noexcept = default;

		DynamicBuffer(allocator_type alloc = {}) noexcept : m_data{ alloc } {}

		DynamicBuffer(storage_type const & data, allocator_type alloc = {}) : m_data{ data, alloc } {}

		DynamicBuffer(storage_type && data, allocator_type alloc = {}) noexcept : m_data{ std::move(data), alloc } {}

		DynamicBuffer(size_t const size_in_bytes, byte const value = null, allocator_type alloc = {}) : m_data{ alloc } { resize(size_in_bytes, value); }

		DynamicBuffer(void const * src, size_t const size_in_bytes, allocator_type alloc = {}) : m_data{ alloc } { write(src, size_in_bytes); }

		template <class Iter = iterator
		> DynamicBuffer(Iter first, Iter last, allocator_type alloc = {}) : m_data{ alloc } { write(first, std::distance(first, last) / sizeof(*first)); }

		DynamicBuffer(DynamicBuffer const & other, allocator_type alloc = {}) : m_data{ other.m_data, alloc } {}

		DynamicBuffer & operator=(DynamicBuffer const & other) { DynamicBuffer temp{ other }; return swap(temp); }

	public:
		NODISCARD bool empty() const noexcept { return m_data.empty(); }

		NODISCARD auto size() const noexcept -> size_t { return m_data.size(); }

		NODISCARD auto capacity() const noexcept -> size_t { return m_data.capacity(); }

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

		NODISCARD operator storage_type & () & noexcept { return m_data; }

		NODISCARD operator storage_type const & () const & noexcept { return m_data; }

		NODISCARD operator storage_type && () && noexcept { return std::move(m_data); }

	public:
		DynamicBuffer & clear() noexcept
		{
			return m_data.clear(), (*this);
		}

		DynamicBuffer & copy(DynamicBuffer const & other) {
			if (this != std::addressof(other)) { m_data = other.m_data; }
			return (*this);
		}

		DynamicBuffer & reserve(size_t const size_in_bytes)
		{
			if (size() < size_in_bytes)
			{
				resize(size_in_bytes, null);
			}
			return (*this);
		}

		DynamicBuffer & resize(size_t const size_in_bytes, byte const value = null)
		{
			return m_data.resize(size_in_bytes, value), (*this);
		}

		DynamicBuffer & swap(DynamicBuffer & other) noexcept
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

	public:
		void write_unchecked(size_t const index, void const * src, size_t const size_in_bytes)
		{
			reserve(index + size_in_bytes);

			copymem(begin() + index, src, size_in_bytes);
		}

		DynamicBuffer & write(size_t const index, void const * src, size_t const size_in_bytes)
		{
			if (src && size_in_bytes) { write_unchecked(index, src, size_in_bytes); }
			return (*this);
		}

		DynamicBuffer & write(void const * src, size_t const size_in_bytes)
		{
			if (src && size_in_bytes) { write_unchecked(size(), src, size_in_bytes); }
			return (*this);
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> DynamicBuffer & write(size_t const index, T const & value)
		{
			write_unchecked(index, std::addressof(value), sizeof(T));
			return (*this);
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> DynamicBuffer & write(T const & value)
		{
			write_unchecked(size(), std::addressof(value), sizeof(T));
			return (*this);
		}

	public:
		DynamicBuffer & vprintf(size_t const index, cstring fmt, va_list args)
		{
			if (int32_t size_in_bytes{ std::vsnprintf(nullptr, 0, fmt, args) }; 0 < size_in_bytes)
			{
				size_in_bytes++; // account for null teminator

				reserve(index + (size_t)size_in_bytes);

				std::vsprintf((char *)begin() + index, fmt, args);
			}
			return (*this);
		}

		DynamicBuffer & vprintf(cstring fmt, va_list args)
		{
			return vprintf(size(), fmt, args);
		}

		DynamicBuffer & printf(size_t const index, cstring fmt, ...)
		{
			if (!fmt) { return (*this); }
			va_list args;
			va_start(args, fmt);
			vprintf(index, fmt, args);
			va_end(args);
			return (*this);
		}

		DynamicBuffer & printf(cstring fmt, ...)
		{
			if (!fmt) { return (*this); }
			va_list args;
			va_start(args, fmt);
			vprintf(size(), fmt, args);
			va_end(args);
			return (*this);
		}

		DynamicBuffer & print(size_t const index, String const & str)
		{
			if (!str.empty()) { write_unchecked(index, str.data(), str.size()); }
			return (*this);
		}

		DynamicBuffer & print(String const & str)
		{
			if (!str.empty()) { write_unchecked(size(), str.data(), str.size()); }
			return (*this);
		}

		DynamicBuffer & print(size_t const index, cstring str, size_t const size_in_bytes)
		{
			if (str && size_in_bytes) { write_unchecked(index, str, size_in_bytes); }
			return (*this);
		}

		DynamicBuffer & print(size_t const index, cstring str)
		{
			if (str) { write_unchecked(index, str, std::strlen(str)); }
			return (*this);
		}

		DynamicBuffer & print(cstring str, size_t const size_in_bytes)
		{
			if (str && size_in_bytes) { write_unchecked(size(), str, size_in_bytes); }
			return (*this);
		}

		DynamicBuffer & print(cstring str)
		{
			if (str) { write_unchecked(size(), str, std::strlen(str)); }
			return (*this);
		}

	public:
		template <class T
		> DynamicBuffer & operator<<(T const & value)
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

		DynamicBuffer & operator<<(cstring str)
		{
			return print(str);
		}

		DynamicBuffer & operator<<(String const & str)
		{
			return print(str);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <size_t _Size
	> class StaticBuffer
	{
		static_assert(0 < _Size);

		byte m_data[_Size]{};

	public:
		using self_type			= typename StaticBuffer<_Size>;
		using reference			= typename byte &;
		using const_reference	= typename byte const &;
		using pointer			= typename byte *;
		using const_pointer		= typename byte const *;
		using iterator			= typename pointer;
		using const_iterator	= typename const_pointer;

		static constexpr byte null{ (byte)'\0' };

		StaticBuffer() noexcept {}

		StaticBuffer(self_type const & other) { copy(other); }
	
		StaticBuffer(self_type && other) noexcept { swap(std::move(other)); }

		StaticBuffer(size_t const index, void const * src, size_t const size_in_bytes) noexcept {
			write(index, src, size_in_bytes);
		}

		StaticBuffer(void const * src, size_t const size_in_bytes) noexcept {
			write(0, src, size_in_bytes);
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> StaticBuffer(size_t const index, T const & value) noexcept {
			write(index, value);
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> StaticBuffer(T const & value) noexcept {
			write(0, value);
		}
	
		self_type & operator=(self_type const & other) {
			self_type temp{ other };
			return swap(temp);
		}
	
		self_type & operator=(self_type && other) noexcept {
			return swap(std::move(other));
		}

	public:
		self_type & clear() noexcept {
			zeromem(m_data, _Size);
			return (*this);
		}
	
		self_type & copy(self_type const & other) {
			if (this != std::addressof(other)) { copymem(m_data, other.m_data, _Size); }
			return (*this);
		}
	
		self_type & swap(self_type & other) noexcept {
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

		NODISCARD auto size() const noexcept -> size_t { return _Size; }

		NODISCARD auto capacity() const noexcept -> size_t { return _Size; }

		NODISCARD auto data() noexcept -> pointer { return m_data; }

		NODISCARD auto data() const noexcept -> const_pointer { return m_data; }

		NODISCARD auto c_str() const noexcept -> cstring { return (cstring)data(); }

		NODISCARD auto begin() noexcept -> iterator { return data(); }

		NODISCARD auto begin() const noexcept -> const_iterator { return data(); }

		NODISCARD auto end() noexcept -> iterator { return begin() + size(); }

		NODISCARD auto end() const noexcept -> const_iterator { return begin() + size(); }

		NODISCARD auto front() noexcept -> reference { return *(begin()); }

		NODISCARD auto front() const noexcept -> const_reference { return *(begin()); }

		NODISCARD auto back() noexcept -> reference { return *(end() - 1); }

		NODISCARD auto back() const noexcept -> const_reference { return *(end() - 1); }

		NODISCARD auto operator[](size_t const i) noexcept -> reference { return m_data[i]; }

		NODISCARD auto operator[](size_t const i) const noexcept -> byte { return m_data[i]; }

		NODISCARD operator void * () noexcept { return m_data; }

		NODISCARD operator void const * () const noexcept { return m_data; }

	public:
		void * get_to(size_t const index, void * dst, size_t const size_in_bytes)
		{
			ASSERT(index + size_in_bytes <= _Size);
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

	public:
		void write_unchecked(size_t const index, void const * src, size_t const size_in_bytes)
		{
			ASSERT(index + size_in_bytes <= _Size);

			copymem(begin() + index, src, size_in_bytes);
		}

		self_type & write(size_t const index, void const * src, size_t const size_in_bytes)
		{
			if (src && size_in_bytes) { write_unchecked(index, src, size_in_bytes); }
			return (*this);
		}

		self_type & write(void const * src, size_t const size_in_bytes)
		{
			if (src && size_in_bytes) { write_unchecked(0, src, size_in_bytes); }
			return (*this);
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> self_type & write(size_t const index, T const & value)
		{
			write_unchecked(index, std::addressof(value), sizeof(T));
			return (*this);
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> self_type & write(T const & value)
		{
			write_unchecked(0, std::addressof(value), sizeof(T));
			return (*this);
		}

	public:
		self_type & vprintf(size_t const index, cstring fmt, va_list args)
		{
			if (int32_t size_in_bytes{ std::vsnprintf(nullptr, 0, fmt, args) }; 0 < size_in_bytes)
			{
				size_in_bytes++; // account for null teminator

				ASSERT(index + size_in_bytes <= _Size);

				std::vsprintf((char *)begin() + index, fmt, args);
			}
			return (*this);
		}

		self_type & vprintf(cstring fmt, va_list args)
		{
			return vprintf(0, fmt, args);
		}

		self_type & printf(size_t const index, cstring fmt, ...)
		{
			if (!fmt) { return (*this); }
			va_list args;
			va_start(args, fmt);
			vprintf(index, fmt, args);
			va_end(args);
			return (*this);
		}

		self_type & printf(cstring fmt, ...)
		{
			if (!fmt) { return (*this); }
			va_list args;
			va_start(args, fmt);
			vprintf(0, fmt, args);
			va_end(args);
			return (*this);
		}

		self_type & print(size_t const index, String const & str)
		{
			if (!str.empty()) { write_unchecked(index, str.data(), str.size()); }
			return (*this);
		}

		self_type & print(String const & str)
		{
			if (!str.empty()) { write_unchecked(0, str.data(), str.size()); }
			return (*this);
		}

		self_type & print(size_t const index, cstring str, size_t const size_in_bytes)
		{
			if (str && size_in_bytes) { write_unchecked(index, str, size_in_bytes); }
			return (*this);
		}

		self_type & print(size_t const index, cstring str)
		{
			if (str) { write_unchecked(index, str, std::strlen(str)); }
			return (*this);
		}

		self_type & print(cstring str, size_t const size_in_bytes)
		{
			if (str && size_in_bytes) { write_unchecked(0, str, size_in_bytes); }
			return (*this);
		}

		self_type & print(cstring str)
		{
			if (str) { write_unchecked(0, str, std::strlen(str)); }
			return (*this);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	template <size_t _Alignment, class ... _Types
	> class ConstantBuffer
	{
		static_assert(!_Alignment || _Alignment % 2 == 0);

		static_assert(0 < sizeof...(_Types));

		template <size_t I
		> using _type = mpl::nth<I, mpl::type_list<_Types...>>;

		template <size_t I = 0, size_t Result = 0
		> static constexpr size_t _calculate_size() noexcept
		{
			if constexpr (I < sizeof...(_Types))
			{
				return _calculate_size<I + 1, Result + util::align_up(sizeof(_type<I>), _Alignment)>();
			}
			else
			{
				return Result;
			}
		}

		template <size_t Index, size_t I = 0, size_t Result = 0
		> static constexpr size_t _calculate_index() noexcept
		{
			static_assert(Index < sizeof...(_Types));

			if constexpr (I < Index)
			{
				return _calculate_index<Index, I + 1, Result + util::align_up(sizeof(_type<I>), _Alignment)>();
			}
			else
			{
				return Result;
			}
		}

		static constexpr size_t _Size{ _calculate_size() };

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

		static constexpr byte null{ (byte)'\0' };

		ConstantBuffer() noexcept {}

		ConstantBuffer(self_type const & other) { copy(other); }

		ConstantBuffer(self_type && other) noexcept { swap(std::move(other)); }

		self_type & operator=(self_type const & other) { self_type temp{ other }; return swap(temp); }

		self_type & operator=(self_type && other) noexcept { return swap(std::move(other)); }

	public:
		self_type & clear() noexcept {
			zeromem(m_data, _Size);
			return (*this);
		}

		self_type & copy(self_type const & other) {
			if (this != std::addressof(other)) { copymem(m_data, other.m_data, _Size); }
			return (*this);
		}

		self_type & swap(self_type & other) noexcept {
			if (this != std::addressof(other)) {
				byte temp[_Size];
				copymem(temp, m_data, _Size);
				copymem(m_data, other.m_data, _Size);
				copymem(other.m_data, temp, _Size);
			}
			return (*this);
		}

	public:
		NODISCARD auto alignment() const noexcept { return _Alignment; }

		NODISCARD bool empty() const noexcept { return false; }

		NODISCARD auto size() const noexcept -> size_t { return _Size; }

		NODISCARD auto capacity() const noexcept -> size_t { return _Size; }

		NODISCARD auto data() noexcept -> pointer { return m_data; }

		NODISCARD auto data() const noexcept -> const_pointer { return m_data; }

		NODISCARD auto c_str() const noexcept -> cstring { return (cstring)data(); }

		NODISCARD auto begin() noexcept -> iterator { return data(); }

		NODISCARD auto begin() const noexcept -> const_iterator { return data(); }

		NODISCARD auto end() noexcept -> iterator { return begin() + size(); }

		NODISCARD auto end() const noexcept -> const_iterator { return begin() + size(); }

		NODISCARD auto front() noexcept -> reference { return *(begin()); }

		NODISCARD auto front() const noexcept -> const_reference { return *(begin()); }

		NODISCARD auto back() noexcept -> reference { return *(end() - 1); }

		NODISCARD auto back() const noexcept -> const_reference { return *(end() - 1); }

		NODISCARD auto operator[](size_t const i) noexcept -> reference { return m_data[i]; }

		NODISCARD auto operator[](size_t const i) const noexcept -> byte { return m_data[i]; }

		NODISCARD operator void * () noexcept { return m_data; }

		NODISCARD operator void const * () const noexcept { return m_data; }

		NODISCARD operator StaticBuffer<_Size> & () noexcept { return *((StaticBuffer<_Size> *)this); }

		NODISCARD operator StaticBuffer<_Size> const & () const noexcept { return *((StaticBuffer<_Size> *)this); }

		NODISCARD operator DynamicBuffer() const noexcept { return { data(), size() }; }

	public:
		void * get_to(size_t const index, void * dst, size_t const size_in_bytes)
		{
			ASSERT(index + size_in_bytes <= _Size);
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

		template <size_t I
		> NODISCARD auto get() -> _type<I>
		{
			return get<_type<I>>(_calculate_index<I>());
		}

	public:
		void write_unchecked(size_t const index, void const * src, size_t const size_in_bytes)
		{
			ASSERT(index + size_in_bytes <= _Size);

			copymem(begin() + index, src, size_in_bytes);
		}

		self_type & write(size_t const index, void const * src, size_t const size_in_bytes)
		{
			if (src && size_in_bytes) { write_unchecked(index, src, size_in_bytes); }
			return (*this);
		}

		self_type & write(void const * src, size_t const size_in_bytes)
		{
			if (src && size_in_bytes) { write_unchecked(0, src, size_in_bytes); }
			return (*this);
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> self_type & write(size_t const index, T const & value)
		{
			return write_unchecked(index, std::addressof(value), sizeof(T)), (*this);
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> self_type & write(T const & value)
		{
			return write_unchecked(0, std::addressof(value), sizeof(T)), (*this);
		}

		template <size_t I
		> self_type & set(_type<I> const & value)
		{
			return write(_calculate_index<I>(), value);
		}

	public:
		self_type & vprintf(size_t const index, cstring fmt, va_list args)
		{
			if (int32_t size_in_bytes{ std::vsnprintf(nullptr, 0, fmt, args) }; 0 < size_in_bytes)
			{
				size_in_bytes++; // account for null teminator

				ASSERT(index + size_in_bytes <= _Size);

				std::vsprintf((char *)begin() + index, fmt, args);
			}
			return (*this);
		}

		self_type & vprintf(cstring fmt, va_list args)
		{
			return vprintf(0, fmt, args);
		}

		self_type & printf(size_t const index, cstring fmt, ...)
		{
			if (!fmt) { return (*this); }
			va_list args;
			va_start(args, fmt);
			vprintf(index, fmt, args);
			va_end(args);
			return (*this);
		}

		self_type & printf(cstring fmt, ...)
		{
			if (!fmt) { return (*this); }
			va_list args;
			va_start(args, fmt);
			vprintf(0, fmt, args);
			va_end(args);
			return (*this);
		}

		self_type & print(size_t const index, String const & str)
		{
			if (!str.empty()) { write_unchecked(index, str.data(), str.size()); }
			return (*this);
		}

		self_type & print(String const & str)
		{
			if (!str.empty()) { write_unchecked(0, str.data(), str.size()); }
			return (*this);
		}

		self_type & print(size_t const index, cstring str, size_t const size_in_bytes)
		{
			if (str && size_in_bytes) { write_unchecked(index, str, size_in_bytes); }
			return (*this);
		}

		self_type & print(size_t const index, cstring str)
		{
			if (str) { write_unchecked(index, str, std::strlen(str)); }
			return (*this);
		}

		self_type & print(cstring str, size_t const size_in_bytes)
		{
			if (str && size_in_bytes) { write_unchecked(0, str, size_in_bytes); }
			return (*this);
		}

		self_type & print(cstring str)
		{
			if (str) { write_unchecked(0, str, std::strlen(str)); }
			return (*this);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_BUFFER_HPP_
