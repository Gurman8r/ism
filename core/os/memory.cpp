#include <core/os/memory.hpp>
#include <core/os/os.hpp>
#include <core/templates/batch.hpp>
#include <core/error/error_macros.hpp>

#ifndef ISM_MEMORY_LEAK_DISPLAY
#define ISM_MEMORY_LEAK_DISPLAY 1
#endif

#ifndef ISM_MEMORY_LEAK_CLEANUP
#define ISM_MEMORY_LEAK_CLEANUP 0
#endif

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

enum : size_t { ID_index, ID_size, ID_addr, ID_desc };

static struct NODISCARD MemoryTracker final
{
	size_t index{};

	Batch<size_t, size_t, void *, cstring> records{};

	~MemoryTracker()
	{
#if ISM_MEMORY_LEAK_DISPLAY
		if (!records.empty())
		{
			get_os().print("\n\nMEMORY LEAKS DETECTED:");

			for (size_t i = 0; i < records.size(); ++i)
			{
				records.expand_all(i, [&](size_t index, size_t size, void * addr, cstring desc)
				{
					get_os().print("\nindex:%zu, size:%zu, addr:%p, desc: %s", index, size, addr, desc);
				});
			}

			get_os().pause();
		}
#endif

#if ISM_MEMORY_LEAK_CLEANUP
		while (!records.empty()) { memfree(records.back<ID_addr>()); }
#else
		VERIFY("MEMORY LEAKS DETECTED" && g_memory.records.empty());
#endif
	}
}
g_memory{};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void * Memory::alloc_static(size_t size, cstring desc)
{
	std::pmr::memory_resource * const mres{ std::pmr::get_default_resource() };

	return std::get<ID_addr>(g_memory.records.push_back
	(
		++g_memory.index, size, mres->allocate(size), desc
	));
}

void * Memory::realloc_static(void * ptr, size_t oldsz, size_t newsz)
{
	if (newsz == 0)
	{
		free_static(ptr);

		return nullptr;
	}
	else if (ptr == nullptr)
	{
		return alloc_static(newsz);
	}
	else if (newsz <= oldsz)
	{
		return ptr;
	}
	else
	{
		void * temp{ alloc_static(newsz) };
		if (temp)
		{
			copymem(temp, ptr, oldsz);

			free_static(ptr);
		}
		return temp;
	}
}

void Memory::free_static(void * ptr)
{
	std::pmr::memory_resource * const mres{ std::pmr::get_default_resource() };

	if (size_t const i{ g_memory.records.index_of<ID_addr>(ptr) }
	; i != g_memory.records.npos)
	{
		mres->deallocate(ptr, g_memory.records.get<ID_size>(i));

		g_memory.records.erase(i);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void * operator new(size_t size, ism::cstring desc)
{
	return ism::Memory::alloc_static(size, desc);
}

void * operator new(size_t size, void * (*alloc_fn)(size_t))
{
	return alloc_fn(size);
}

void operator delete(void * ptr, ism::cstring desc)
{
	FATAL("this should never be called");
}

void operator delete(void * ptr, void * (*alloc_fn)(size_t))
{
	FATAL("this should never be called");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */