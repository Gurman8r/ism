#ifndef _ISM_MEMORY_HPP_
#define _ISM_MEMORY_HPP_

#include <core/os/copymem.hpp>

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

#define memnew(T) (::ism::_post_initialize(new ("") T))

#define memnew_placement(ptr, T) (::ism::_post_initialize(new (ptr, sizeof(T), "") T))

	template <class T, class ... Args
	> NODISCARD auto construct_or_initialize(Args && ... args) -> T *
	{
		if constexpr (0 == sizeof...(Args))
		{
			return memnew(T);
		}
		else if constexpr (std::is_constructible_v<T>)
		{
			return memnew(T(FWD(args)...));
		}
		else
		{
			return memnew(T{ FWD(args)... });
		}
	}

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
	class DefaultAllocator final
	{
	public:
		constexpr DefaultAllocator() noexcept = default;

		static void * allocate(size_t size) { return memalloc(size); }

		static void deallocate(void * ptr) { memfree(ptr); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Global Delete
	struct GlobalDelete final
	{
		void operator()(void * const ptr) const
		{
			::operator delete(ptr);
		}
	};

	// Default Delete
	template <class T> struct DefaultDelete
	{
		template <class U> void operator()(U * value) const
		{
			memdelete((T *)value);
		}
	};

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
		constexpr NoDelete() = default;

		template <class U> void operator()(U *) const {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> ALIAS(Shared) std::shared_ptr<T>;

	template <class T, class ... Args
	> NODISCARD auto make_shared(Args && ... args)
	{
		return std::make_shared<T>(FWD(args)...);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class Dx = DefaultDelete<T>
	> ALIAS(Unique) std::unique_ptr<T, Dx>; // unique pointer

	template <class T, class Dx = DefaultDelete<T>
	> NODISCARD auto make_scope() -> Unique<T, Dx>
	{
		return { memnew(T), Dx{} };
	}

	template <class T, class Dx = DefaultDelete<T>
	> NODISCARD auto make_scope(T * ptr) -> Unique<T, Dx>
	{
		return { ptr, Dx{} };
	}

	template <class T, class Dx
	> NODISCARD auto make_scope(T * ptr, Dx && dx) -> Unique<T, Dx>
	{
		return { ptr, FWD(dx) };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> ALIAS(Scary) std::unique_ptr<T, NoDelete>; // non-deleting unique-pointer

	template <class T, class ... Args
	> NODISCARD auto make_scary(Args && ... args) -> Scary<T>
	{
		return { memnew((T)(FWD(args)...)), NoDelete{} };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> using is_shared_ptr = mpl::is_instantiation<std::shared_ptr, T>;

	template <class T> constexpr bool is_shared_ptr_v{ is_shared_ptr<T>::value };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class TestResource : public std::pmr::memory_resource
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		TestResource(std::pmr::memory_resource * mres, void * data, size_t size) noexcept
			: m_resource	{ mres }
			, m_buffer_data	{ (byte *)data }
			, m_buffer_size	{ size }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD bool is_default() const noexcept { return this == std::pmr::get_default_resource(); }

		NODISCARD auto get_resource() const noexcept -> std::pmr::memory_resource * const { return m_resource; }

		NODISCARD auto num_allocations() const noexcept -> size_t { return m_num_allocations; }

		NODISCARD auto data() const noexcept -> pointer { return m_buffer_data; }

		NODISCARD auto size() const noexcept -> size_t { return m_buffer_size; }

		NODISCARD auto bytes_used() const noexcept -> size_t { return m_buffer_used; }

		NODISCARD auto bytes_free() const noexcept -> size_t { return m_buffer_size - m_buffer_used; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD auto front() & noexcept -> reference { return *begin(); }

		NODISCARD auto front() const & noexcept -> const_reference { return *cbegin(); }

		NODISCARD auto back() & noexcept -> reference { return *(end() - 1); }

		NODISCARD auto back() const & noexcept -> const_reference { return *(cend() - 1); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		void * do_allocate(size_t bytes, size_t align) override
		{
			++m_num_allocations;
			m_buffer_used += bytes;
			return m_resource->allocate(bytes, align);
		}

		void do_deallocate(void * ptr, size_t bytes, size_t align) override
		{
			--m_num_allocations;
			m_buffer_used -= bytes;
			return m_resource->deallocate(ptr, bytes, align);
		}

		bool do_is_equal(std::pmr::memory_resource const & value) const noexcept override
		{
			return m_resource->is_equal(value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		std::pmr::memory_resource * m_resource;
		pointer const m_buffer_data;
		size_t const m_buffer_size;

		size_t m_num_allocations{};
		size_t m_buffer_used{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_MEMORY_HPP_
