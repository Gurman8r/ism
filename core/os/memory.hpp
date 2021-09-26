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

ISM_API_FUNC(void *) operator new(size_t size, ism::cstring desc);

ISM_API_FUNC(void *) operator new(size_t size, void * (*alloc_fn)(size_t));

FORCE_INLINE void * operator new(size_t size, void * ptr, size_t check, ism::cstring desc) { return ptr; }

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_API_FUNC(void) operator delete(void * ptr, ism::cstring desc);

ISM_API_FUNC(void) operator delete(void * ptr, void * (*alloc_fn)(size_t));

FORCE_INLINE void operator delete(void * placement, void * ptr, size_t check, ism::cstring desc) {}

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

#define memnew(T) ::ism::_post_initialize(new ("") T)

#define memnew_placement(ptr, T) ::ism::_post_initialize(new (ptr, sizeof(T), "") T)

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
	> ALIAS(PolymorphicAllocator) _STD pmr::polymorphic_allocator<T>;

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

	template <class T> void default_delete(T * value) noexcept
	{
		DefaultDelete<T>{}(value);
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
}

#endif // !_ISM_MEMORY_HPP_
