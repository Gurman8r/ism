#ifndef _ISM_DICT_OBJECT_HPP_
#define _ISM_DICT_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// dict
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class NODISCARD ISM_API CoreDict : public CoreObject
	{
		ISM_OBJECT_DEFAULT(CoreDict, CoreObject);

	protected:
		static void _bind_methods(CoreType & t);

	protected:
		HashMap<OBJECT, OBJECT> m_dict{};

	public:
		using storage_type = decltype(m_dict);
		using item_type = typename storage_type::value_type;
		using iterator = typename storage_type::iterator;
		using const_iterator = typename storage_type::const_iterator;

		CoreDict(storage_type const & v) : base_type{ &ob_type_static }, m_dict{ v } {}

		CoreDict(storage_type && v) noexcept : base_type{ &ob_type_static }, m_dict{ std::move(v) } {}

		CoreDict(std::initializer_list<item_type> init) : self_type{}
		{
			m_dict.reserve(init.size());

			for (item_type const & e : init) { m_dict.insert(e); }
		}

		NODISCARD operator storage_type * () const { return const_cast<storage_type *>(&m_dict); }
		
		NODISCARD auto operator->() const { return const_cast<storage_type *>(&m_dict); }

		NODISCARD bool empty() const noexcept { return m_dict.empty(); }
		
		NODISCARD auto size() const noexcept { return m_dict.size(); }

		template <class Index = OBJECT
		> void erase(Index && i) { m_dict.erase(object_forward(FWD(i))); }

		template <class Index = OBJECT
		> bool contains(Index && i) const { return end() != m_dict.find(object_forward(FWD(i))); }

		template <class Index = OBJECT
		> auto find(Index && i) { return m_dict.find(object_forward(FWD(i))); }

		template <class Index = OBJECT, class Value = OBJECT
		> void insert(Index && i, Value && v) { m_dict.try_emplace(object_forward(FWD(i)), object_forward(FWD(v))); }

		template <class Index = OBJECT
		> auto get(Index && i) { return m_dict[object_forward(FWD(i))]; }

		template <class Index = OBJECT, class Value = OBJECT
		> void set(Index && i, Value && v) { m_dict[object_forward(FWD(i))] = object_forward(FWD(i)); }

		NODISCARD auto begin() noexcept -> iterator { return m_dict.begin(); }
		
		NODISCARD auto begin() const noexcept -> const_iterator { return m_dict.begin(); }
		
		NODISCARD auto cbegin() const noexcept -> const_iterator { return m_dict.cbegin(); }
		
		NODISCARD auto end() noexcept -> iterator { return m_dict.end(); }
		
		NODISCARD auto end() const noexcept -> const_iterator { return m_dict.end(); }
		
		NODISCARD auto cend() const noexcept -> const_iterator { return m_dict.cend(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_DICT_OBJECT_HPP_
