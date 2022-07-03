#ifndef _ISM_VECTOR_HPP_
#define _ISM_VECTOR_HPP_

#include <core/os/memory.hpp>
#include <core/io/json.hpp>

#include <forward_list>
#include <vector>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// vector base
	template <class T
	> ALIAS(_VectorBase) std::vector<T, PolymorphicAllocator<T>>;

	// vector
	template <class T
	> class Vector : public _VectorBase<T> {
	public:
		using base_type = _VectorBase<T>;
		using base_type::base_type;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> void to_json(Json & json, Vector<T> const & value)
	{
		json = (std::pmr::vector<T> const &)value;
	}

	template <class T
	> void from_json(Json const & json, Vector<T> & value)
	{
		json.get_to((std::pmr::vector<T> &)value);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// has
	template <class TValue, class TKey
	> bool has(Vector<TValue> const & l, TKey && value)
	{
		return l.end() != std::find(l.begin(), l.end(), FWD(value));
	}

	// getptr
	template <class TValue, class TKey
	> auto getptr(Vector<TValue> const & l, TKey && value)
	{
		if (auto const it{ std::find(l.begin(), l.end(), FWD(value)) }; it != l.end())
		{
			return (TValue *)(void *)std::addressof(*it);
		}
		else
		{
			return nullptr;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_VECTOR_HPP_
