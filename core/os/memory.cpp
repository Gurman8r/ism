#include <core/os/memory.hpp>
#include <core/os/os.hpp>
#include <core/templates/batch.hpp>
#include <core/error/error_macros.hpp>

#ifndef LEAK_DISPLAY_ENABLED
#define LEAK_DISPLAY_ENABLED 1
#endif

#ifndef LEAK_CLEANUP_ENABLED
#define LEAK_CLEANUP_ENABLED 0
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
#if LEAK_DISPLAY_ENABLED
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

			SINGLETON(OS)->pause();
		}
#endif

#if LEAK_CLEANUP_ENABLED
		while (!records.empty()) { memfree(records.back<ID_addr>()); }
#else
		ASSERT("MEMORY LEAKS DETECTED" && g_memory_tracker.records.empty());
#endif
	}
}
g_memory_tracker{};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void * Memory::alloc_static(size_t size, cstring desc)
{
	return std::get<ID_addr>(g_memory_tracker.records.push_back
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
	if (size_t const i{ g_memory_tracker.records.index_of<ID_addr>(ptr) }
	; i != g_memory_tracker.records.npos)
	{
		size_t const size{ g_memory_tracker.records.get<ID_size>(i) };

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

MemoryArena::MemoryArena(size_t const size)
	: m_data	{ size, byte{}, std::allocator<byte>{} }
	, m_buffer	{ m_data.data(), size }
	, m_pool	{ &m_buffer }
	, m_view	{ &m_pool, m_data.data(), size }
	, m_prev	{ std::pmr::get_default_resource() }
{
	std::pmr::set_default_resource(&m_view);
}

MemoryArena::~MemoryArena()
{
	m_pool.release();
	m_buffer.release();
	std::pmr::set_default_resource(m_prev);
}

static struct MemoryArenaStack final
{
	std::vector<MemoryArena *> data{};

	~MemoryArenaStack() { ASSERT(data.empty()); }
}
g_arena_stack{};

MemoryArena * ism::get_current_memory_arena()
{
	if (g_arena_stack.data.empty())
	{
		return nullptr;
	}
	else
	{
		return g_arena_stack.data.back();
	}
}

MemoryArena * ism::push_memory_arena(size_t const size)
{
	void * ptr{ std::pmr::new_delete_resource()->allocate(sizeof(MemoryArena)) };

	return g_arena_stack.data.emplace_back(::new(ptr) MemoryArena{ size });
}

void ism::pop_memory_arena()
{
	ASSERT(!g_arena_stack.data.empty());

	MemoryArena * ptr{ g_arena_stack.data.back() };
	
	ptr->~MemoryArena();
	
	std::pmr::new_delete_resource()->deallocate(ptr, sizeof(MemoryArena));
	
	g_arena_stack.data.pop_back();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */