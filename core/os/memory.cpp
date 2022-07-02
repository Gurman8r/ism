#include <core/os/memory.hpp>
#include <core/os/os.hpp>
#include <core/templates/batch.hpp>
#include <core/error/error_macros.hpp>

#ifndef SHOW_FINAL_ALLOCATIONS
#define SHOW_FINAL_ALLOCATIONS 1
#endif

#ifndef CLEAR_FINAL_ALLOCATIONS
#define CLEAR_FINAL_ALLOCATIONS 0
#endif

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static struct NODISCARD MemoryTracker final
{
	size_t index{};

	enum { ID_Index, ID_Size, ID_Addr, ID_Desc };

	Batch<size_t, size_t, void *, cstring> records{};

	~MemoryTracker()
	{
#if SHOW_FINAL_ALLOCATIONS
		if (!records.empty())
		{
			std::cerr << "\nMEMORY LEAKS DETECTED:\n";

			for (size_t i = 0; i < records.size(); ++i)
			{
				records.expand_all(i, [&](size_t index, size_t size, void * addr, cstring desc)
				{
					std::cerr << "index:" << index << " | size:" << size << " | addr:" << addr << " | desc:\"" << desc << "\"\n";
				});
			}

			SYSTEM->pause();
		}
#endif

#if CLEAR_FINAL_ALLOCATIONS
		while (!records.empty()) { memfree(records.back<ID_Addr>()); }
#else
		ASSERT("MEMORY LEAKS DETECTED" && g_memory_tracker.records.empty());
#endif
	}
}
g_memory_tracker{};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void * Memory::alloc_static(size_t size, cstring desc)
{
	return std::get<MemoryTracker::ID_Addr>(g_memory_tracker.records.push_back
	(
		++g_memory_tracker.index,
		size,
		std::pmr::get_default_resource()->allocate(size),
		desc
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
	if (size_t const i{ g_memory_tracker.records.index_of<MemoryTracker::ID_Addr>(ptr) }
	; i != g_memory_tracker.records.npos)
	{
		size_t const size{ g_memory_tracker.records.get<MemoryTracker::ID_Size>(i) };

		std::pmr::get_default_resource()->deallocate(ptr, size);

		g_memory_tracker.records.erase(i);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void * operator new(size_t size, char const * desc)
{
	return ism::Memory::alloc_static(size, desc);
}

void * operator new(size_t size, void * (*alloc_fn)(size_t))
{
	return alloc_fn(size);
}

void operator delete(void * ptr, char const * desc)
{
	CRASH("this should never be called");
}

void operator delete(void * ptr, void * (*alloc_fn)(size_t))
{
	CRASH("this should never be called");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */