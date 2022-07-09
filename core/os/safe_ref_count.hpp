#ifndef _ISM_SAFE_REFCOUNT_HPP_
#define _ISM_SAFE_REFCOUNT_HPP_

#include <core/templates/utility.hpp>

#include <atomic>

#undef MULTICORE_ENABLED
#define MULTICORE_ENABLED 0
static_assert(!MULTICORE_ENABLED, "thread support not implemented");

#if !MULTICORE_ENABLED
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> FORCE_INLINE static T atomic_conditional_increment(T volatile * value)
	{
		if (*value == 0) { return 0; }
		(*value)++;
		return *value;
	}

	template <class T
	> FORCE_INLINE static T atomic_decrement(T volatile * value)
	{
		(*value)--;
		return *value;
	}

	template <class T
	> FORCE_INLINE static T atomic_increment(T volatile * value)
	{
		(*value)++;
		return *value;
	}

	template <class T, class V
	> FORCE_INLINE static T atomic_sub(T volatile * value, V volatile amount)
	{
		(*value) -= amount;
		return *value;
	}

	template <class T, class V
	> FORCE_INLINE static T atomic_add(T volatile * value, V volatile amount)
	{
		(*value) += amount;
		return *value;
	}

	template <class T, class V
	> FORCE_INLINE static T atomic_exchange_if_greater(T volatile * value, V volatile amount)
	{
		if (amount > *value) { *value = amount; }
		return *value;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}
#else
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	uint32_t atomic_conditional_increment(uint32_t volatile * value)
	{
		return 0;
	}

	uint32_t atomic_decrement(uint32_t volatile * value)
	{
		return 0;
	}

	uint32_t atomic_increment(uint32_t volatile * value)
	{
		return 0;
	}

	uint32_t atomic_sub(uint32_t volatile * value, uint32_t volatile amount)
	{
		return 0;
	}

	uint32_t atomic_add(uint32_t volatile * value, uint32_t volatile amount)
	{
		return 0;
	}

	uint32_t atomic_exchange_if_greater(uint32_t volatile * value, uint32_t volatile amount)
	{
		return 0;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	uint64_t atomic_conditional_increment(uint64_t volatile * value)
	{
		return 0;
	}

	uint64_t atomic_decrement(uint64_t volatile * value)
	{
		return 0;
	}

	uint64_t atomic_increment(uint64_t volatile * value)
	{
		return 0;
	}

	uint64_t atomic_sub(uint64_t volatile * value, uint64_t volatile amount)
	{
		return 0;
	}

	uint64_t atomic_add(uint64_t volatile * value, uint64_t volatile amount)
	{
		return 0;
	}

	uint64_t atomic_exchange_if_greater(uint64_t volatile * value, uint64_t volatile amount)
	{
		return 0;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}
#endif

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API SafeRefCount final
	{
		uint32_t m_count{};

	public:
		FORCE_INLINE void init(uint32_t p_value = 1) { m_count = p_value; }

		FORCE_INLINE bool inc_ref() { return atomic_conditional_increment(&m_count) != 0; } // true on success

		FORCE_INLINE bool dec_ref() { return atomic_decrement(&m_count) == 0; } // true if must be disposed of

		FORCE_INLINE auto refval() -> uint32_t { return atomic_conditional_increment(&m_count); } // nullptr-zero on success

		FORCE_INLINE auto unrefval() -> uint32_t { return atomic_decrement(&m_count); } // 0 if must be disposed of

		FORCE_INLINE auto get() const -> uint32_t { return m_count; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_SAFE_REFCOUNT_HPP_
