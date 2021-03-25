#include <core/templates/atomic.hpp>

#if ISM_MULTICORE

namespace ISM
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

#endif // ISM_MULTICORE
