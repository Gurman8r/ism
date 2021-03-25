#ifndef _ISM_MEMORY_HPP_
#define _ISM_MEMORY_HPP_

#include <core/os/copymem.hpp>

#include <core/templates/utility.hpp>

#include <memory>
#include <memory_resource>

#define GLOBAL_NEW	::new
#define GLOBAL_DEL	::delete

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ISM
{
	class ISM_API Memory final
	{
	public:
		static void * alloc_static(size_t size, cstring desc = "");

		static void * realloc_static(void * ptr, size_t oldsz, size_t newsz);

		static void free_static(void * ptr);
	};
}

#define memalloc (ISM::Memory::alloc_static)

#define memrealloc(ptr, size) ISM::Memory::realloc_static(ptr, size, size)

#define memrealloc_sized(ptr, oldsz, newsz) ISM::Memory::realloc_static(ptr, oldsz, newsz)

#define memfree (ISM::Memory::free_static)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_API_FUNC(void *) operator new(size_t size, ISM::cstring desc);

ISM_API_FUNC(void *) operator new(size_t size, void * (*alloc_fn)(size_t));

FORCE_INLINE void * operator new(size_t size, void * ptr, size_t check, ISM::cstring desc) { return ptr; }

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_API_FUNC(void) operator delete(void * ptr, ISM::cstring desc);

ISM_API_FUNC(void) operator delete(void * ptr, void * (*alloc_fn)(size_t));

FORCE_INLINE void operator delete(void * placement, void * ptr, size_t check, ISM::cstring desc) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ISM
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	FORCE_INLINE void postinitialize_handler(void *) {}

	template <class T> T * _post_initialize(T * value)
	{
		::ISM::postinitialize_handler(value);

		return value;
	}

#define memnew(T) ::ISM::_post_initialize(new ("") T)

#define memnew_placement(ptr, T) ::ISM::_post_initialize(new (ptr, sizeof(T), "") T)

	template <class T, class ... Args
	> NODISCARD T * construct_or_initialize(Args && ... args)
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
		if (!::ISM::predelete_handler(ptr)) { return; }

		if constexpr (!std::is_trivially_destructible_v<T>) { ptr->~T(); }

		memfree(ptr);
	}

#define memdelete_nonzero(ptr)		\
	do {							\
		if (ptr)					\
		{							\
			ISM::memdelete(ptr);	\
		}							\
	} while (0)

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if 1
	// Polymorphic Allocator
	template <class _Ty = byte
	> ALIAS(PolymorphicAllocator) typename std::pmr::polymorphic_allocator<_Ty>;
#else
	// Polymorphic Allocator
	template <class _Ty = byte
	> class PolymorphicAllocator : public std::pmr::polymorphic_allocator<_Ty>
	{
		using _Mybase = std::pmr::polymorphic_allocator<_Ty>;
	public:
		using _Mybase::polymorphic_allocator;
		NODISCARD operator _Mybase & () & noexcept { return static_cast<_Mybase &>(*this); }
		NODISCARD operator _Mybase const & () const & noexcept { return static_cast<_Mybase const &>(*this); }
		NODISCARD operator _Mybase && () && noexcept { return static_cast<_Mybase &&>(std::move(*this)); }
	};
#endif

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
		constexpr GlobalDelete() = default;

		void operator()(void * const ptr) const
		{
			::operator delete(ptr);
		}
	};

	// Default Delete
	template <class T
	> struct DefaultDelete final
	{
		constexpr DefaultDelete() = default;

		template <class U> void operator()(U * value) const
		{
			memdelete((T *)value);
		}
	};

	// No Delete
	struct NoDelete final
	{
		constexpr NoDelete() = default;

		template <class U> void operator()(U *) const {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class _Ty, class _Dx = DefaultDelete<_Ty>
	> ALIAS(Unique) typename std::unique_ptr<_Ty, _Dx>; // unique pointer

	template <class T, class Dx = DefaultDelete<T>, class ... Args
	> NODISCARD Unique<T, Dx> make_scope(Args && ... args)
	{
		return { memnew((T)(FWD(args)...)), Dx{} };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class _Ty
	> ALIAS(Scary) typename std::unique_ptr<_Ty, NoDelete>; // non-deleting pointer

	template <class T, class ... Args
	> NODISCARD Scary<T> make_scary(Args && ... args)
	{
		return { memnew((T)(FWD(args)...)), NoDelete{} };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> using is_shared_ptr = is_instantiation<std::shared_ptr, T>;

	template <class T> constexpr bool is_shared_ptr_v{ is_shared_ptr<T>::value };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_MEMORY_HPP_
