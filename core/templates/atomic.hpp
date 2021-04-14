#ifndef _ISM_SAFE_REFCOUNT_HPP_
#define _ISM_SAFE_REFCOUNT_HPP_

#include <core/templates/utility.hpp>

#include <atomic>

#ifndef ISM_MULTICORE
#define ISM_MULTICORE 0
#endif

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if ISM_MULTICORE
	ISM_API uint32_t atomic_conditional_increment(uint32_t volatile * value);
	ISM_API uint32_t atomic_decrement(uint32_t volatile * value);
	ISM_API uint32_t atomic_increment(uint32_t volatile * value);
	ISM_API uint32_t atomic_sub(uint32_t volatile * value, uint32_t volatile amount);
	ISM_API uint32_t atomic_add(uint32_t volatile * value, uint32_t volatile amount);
	ISM_API uint32_t atomic_exchange_if_greater(uint32_t volatile * value, uint32_t volatile amount);

	ISM_API uint64_t atomic_conditional_increment(uint64_t volatile * value);
	ISM_API uint64_t atomic_decrement(uint64_t volatile * value);
	ISM_API uint64_t atomic_increment(uint64_t volatile * value);
	ISM_API uint64_t atomic_sub(uint64_t volatile * value, uint64_t volatile amount);
	ISM_API uint64_t atomic_add(uint64_t volatile * value, uint64_t volatile amount);
	ISM_API uint64_t atomic_exchange_if_greater(uint64_t volatile * value, uint64_t volatile amount);
#else
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
#endif

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API RefCount final
	{
		uint32_t m_count{};

	public:
		// destroy() is called when weak_count_ drops to zero.

		FORCE_INLINE void init(uint32_t p_value = 1) { m_count = p_value; }

		// true on success
		FORCE_INLINE bool ref() { return atomic_conditional_increment(&m_count) != 0; }

		// nullptr-zero on success
		FORCE_INLINE uint32_t refval() { return atomic_conditional_increment(&m_count); }

		// true if must be disposed of
		FORCE_INLINE bool unref() { return atomic_decrement(&m_count) == 0; }

		// 0 if must be disposed of
		FORCE_INLINE uint32_t unrefval() { return atomic_decrement(&m_count); }

		// nothrow
		FORCE_INLINE uint32_t get() const { return m_count; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_SAFE_REFCOUNT_HPP_
