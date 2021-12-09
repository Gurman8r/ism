#ifndef _ISM_VECTOR_HPP_
#define _ISM_VECTOR_HPP_

#include <core/os/memory.hpp>
#include <core/io/json.hpp>

#include <forward_list>
#include <vector>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// vector
	template <class T
	> ALIAS(Vector) std::pmr::vector<T>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> void to_json(JSON & json, Vector<T> const & value)
	{
		json = (std::pmr::vector<T> const &)value;
	}

	template <class T
	> void from_json(JSON const & json, Vector<T> & value)
	{
		json.get_to((std::pmr::vector<T> &)value);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// has
	template <class _Ty, class T
	> NODISCARD bool has(Vector<_Ty> const & l, T && value)
	{
		return l.end() != std::find(l.begin(), l.end(), FWD(value));
	}

	// get pointer
	template <class _Ty, class T
	> NODISCARD auto getptr(Vector<_Ty> const & l, T && value)
	{
		if (auto const it{ std::find(l.begin(), l.end(), FWD(value)) }; it != l.end())
		{
			return (_Ty *)(void *)std::addressof(*it);
		}
		else
		{
			return nullptr;
		}
	}

	// buffer copy
	template <class Elem = byte
	> Vector<byte> bufcpy(size_t count, void const * data, PolymorphicAllocator<> alloc = {}) noexcept
	{
		return (data
			? Vector<byte>{ (byte *)data, (byte *)data + count * sizeof(Elem), alloc }
			: Vector<byte>{ count * sizeof(Elem), (byte)0, alloc });
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_VECTOR_HPP_
