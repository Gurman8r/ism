#ifndef _ISM_MEMORY_HPP_
#define _ISM_MEMORY_HPP_

#include <core/templates/utility.hpp>

#include <memory>
#include <memory_resource>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ism
{
	class ISM_API Memory final
	{
	public:
		static void * alloc_static(size_t size, cstring desc = "");

		static void * realloc_static(void * ptr, size_t oldsz, size_t newsz);

		static void free_static(void * ptr);
	};
}

#define memalloc (ism::Memory::alloc_static)

#define memrealloc(ptr, size) (ism::Memory::realloc_static(ptr, size, size))

#define memrealloc_sized(ptr, oldsz, newsz) (ism::Memory::realloc_static(ptr, oldsz, newsz))

#define memfree (ism::Memory::free_static)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_API_FUNC(void *) operator new(size_t size, char const * desc);

ISM_API_FUNC(void *) operator new(size_t size, void * (*alloc_fn)(size_t));

FORCE_INLINE void * operator new(size_t size, void * ptr, size_t check, char const * desc) { return ptr; }

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_API_FUNC(void) operator delete(void * ptr, char const * desc);

ISM_API_FUNC(void) operator delete(void * ptr, void * (*alloc_fn)(size_t));

FORCE_INLINE void operator delete(void * placement, void * ptr, size_t check, char const * desc) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	FORCE_INLINE void postinitialize_handler(void *) {}

	template <class T> T * _post_initialize(T * value)
	{
		::ism::postinitialize_handler(value);

		return value;
	}

#ifndef MEMNEW_DESC
#define MEMNEW_DESC(T) TOSTR(T)
#endif

#define memnew(T) (::ism::_post_initialize(new (MEMNEW_DESC(T)) T))

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	FORCE_INLINE bool predelete_handler(void *) { return true; }

	template <class T> void memdelete(T * ptr)
	{
		if (!::ism::predelete_handler(ptr)) { return; }

		if constexpr (!std::is_trivially_destructible_v<T>) { ptr->~T(); }

		memfree(ptr);
	}

#define memdelete_nonzero(ptr)		\
	do {							\
		if (ptr)					\
		{							\
			ism::memdelete(ptr);	\
		}							\
	} while (0)

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Polymorphic Allocator
	template <class T = byte
	> ALIAS(PolymorphicAllocator) std::pmr::polymorphic_allocator<T>;

	// Default Allocator
	struct DefaultAllocator final
	{
		static void * allocate(size_t size) { return memalloc(size); }

		static void deallocate(void * ptr) { memfree(ptr); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// global delete
	struct GlobalDelete final
	{
		void operator()(void * const ptr) const
		{
			::operator delete(ptr);
		}
	};

	// default delete
	template <class T> struct DefaultDelete
	{
		template <class U> void operator()(U * value) const
		{
			memdelete((T *)value);
		}
	};

	// default delete (void)
	template <> struct DefaultDelete<void>
	{
		void operator()(void * value) const
		{
			memfree(value);
		}
	};

	template <class T> void call_default_delete(T * value) noexcept
	{
		DefaultDelete<mpl::intrinsic_t<decltype(value)>>{}(value);
	}

	// No Delete
	struct NoDelete final
	{
		template <class U> void operator()(U *) const {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// passthrough for getting information about an upstream memory resource
	class TestMemoryResource : public std::pmr::memory_resource
	{
	public:
		using pointer					= typename byte *;
		using const_pointer				= typename byte const *;
		using reference					= typename byte &;
		using const_reference			= typename byte const &;
		using iterator					= typename pointer;
		using const_iterator			= typename const_pointer;
		using reverse_iterator			= typename std::reverse_iterator<iterator>;
		using const_reverse_iterator	= typename std::reverse_iterator<const_iterator>;
		using size_type					= typename size_t;
		using difference_type			= typename ptrdiff_t;

		TestMemoryResource(std::pmr::memory_resource * mres, void const * data, size_t const size) noexcept
			: m_upstream_resource{ mres }
			, m_buffer_data{ (byte *)data }
			, m_buffer_size{ size }
		{
		}

		NODISCARD bool is_default_resource() const noexcept { return this == std::pmr::get_default_resource(); }
		NODISCARD auto upstream_resource() const noexcept -> std::pmr::memory_resource * const { return m_upstream_resource; }
		NODISCARD auto data() const noexcept -> pointer { return m_buffer_data; }
		NODISCARD auto size() const noexcept -> size_t { return m_buffer_size; }
		NODISCARD auto num_allocations() const noexcept -> size_t { return m_num_allocations; }
		NODISCARD auto bytes_used() const noexcept -> size_t { return m_bytes_used; }
		NODISCARD auto bytes_free() const noexcept -> size_t { return m_buffer_size - m_bytes_used; }

		NODISCARD auto begin() noexcept -> iterator { return m_buffer_data; }
		NODISCARD auto begin() const noexcept -> const_iterator { return m_buffer_data; }
		NODISCARD auto cbegin() const noexcept -> const_iterator { return begin(); }
		NODISCARD auto end() noexcept -> iterator { return m_buffer_data + m_buffer_size; }
		NODISCARD auto end() const noexcept -> const_iterator { return m_buffer_data + m_buffer_size; }
		NODISCARD auto cend() const noexcept -> const_iterator { return end(); }
		NODISCARD auto rbegin() noexcept -> reverse_iterator { return std::make_reverse_iterator(end()); }
		NODISCARD auto rbegin() const noexcept -> const_reverse_iterator { return std::make_reverse_iterator(end()); }
		NODISCARD auto crbegin() const noexcept -> const_reverse_iterator { return rbegin(); }
		NODISCARD auto rend() noexcept -> reverse_iterator { return std::make_reverse_iterator(begin()); }
		NODISCARD auto rend() const noexcept -> const_reverse_iterator { return std::make_reverse_iterator(begin()); }
		NODISCARD auto crend() const noexcept -> const_reverse_iterator { return crend(); }

	private:
		void * do_allocate(size_t bytes, size_t align) override
		{
			++m_num_allocations;
			m_bytes_used += bytes;
			return m_upstream_resource->allocate(bytes, align);
		}

		void do_deallocate(void * ptr, size_t bytes, size_t align) override
		{
			--m_num_allocations;
			m_bytes_used -= bytes;
			return m_upstream_resource->deallocate(ptr, bytes, align);
		}

		bool do_is_equal(std::pmr::memory_resource const & value) const noexcept override
		{
			return m_upstream_resource->is_equal(value);
		}

	private:
		std::pmr::memory_resource * const m_upstream_resource;
		pointer const m_buffer_data;
		size_t const m_buffer_size;
		size_t m_num_allocations{}, m_bytes_used{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_MEMORY_HPP_
