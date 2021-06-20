#ifndef _ISM_STRING_OBJECT_HPP_
#define _ISM_STRING_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// string
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// string object
	class ISM_API StringObject : public BaseObject
	{
		ISM_OBJECT_DEFAULT(StringObject, BaseObject);

	protected:
		static void _bind_class(TypeObject & t);

	public:
		String m_string{};

		using storage_type = decltype(m_string);

		using iterator = storage_type::iterator;

		using const_iterator = storage_type::const_iterator;

		NODISCARD auto & operator*() const { return const_cast<storage_type &>(m_string); }

		NODISCARD auto * operator->() const { return const_cast<storage_type *>(&m_string); }

		StringObject(storage_type const & v) : base_type{ get_type_static() }, m_string{ v } {}

		StringObject(storage_type && v) noexcept : base_type{ get_type_static() }, m_string{ std::move(v) } {}

		StringObject(cstring v) : base_type{ get_type_static() }, m_string{ v } {}

		StringObject(cstring v, size_t n) : base_type{ get_type_static() }, m_string{ v, n } {}

		StringObject(StringName const & v) : self_type{ v.string() } {}

		StringObject(StringName && v) noexcept : self_type{ std::move(v).string() } {}

		StringObject(std::initializer_list<char> init) : self_type{ storage_type{ init.begin(), init.end() } } {}

		template <class T> StringObject(Handle<T> const & o) : self_type{}
		{
			if constexpr (std::is_same_v<T, self_type>)
			{
				m_string = (String)o;
			}
			else if (STR::check_(o))
			{
				m_string = (String)STR(o);
			}
			else if (TYPE t{ typeof(o) }; t && t->tp_str)
			{
				m_string = (String)t->tp_str(o);
			}
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// string delete
	template <> struct DefaultDelete<StringObject> : DefaultDelete<BaseObject> {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// string check
#define ISM_STR_CHECK(o) (typeof(o).has_feature(TypeFlags_Str_Subclass))

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// string handle
	template <> class Handle<StringObject> : public BaseHandle<StringObject>
	{
		ISM_HANDLE_DEFAULT(StringObject, ISM_STR_CHECK);

	public:
		using storage_type = StringObject::storage_type;

		using iterator = StringObject::iterator;

		using const_iterator = StringObject::const_iterator;

		void reserve(size_t count) { (**m_ptr).reserve(count); }

		void resize(size_t count) { (**m_ptr).resize(count); }

		NODISCARD operator storage_type () const { return m_ptr ? (**m_ptr) : String{}; }

		NODISCARD auto c_str() const { return (**m_ptr).c_str(); }

		NODISCARD auto data() const { return (**m_ptr).data(); }

		NODISCARD bool empty() const { return (**m_ptr).empty(); }

		NODISCARD auto size() const { return (**m_ptr).size(); }

		NODISCARD auto front() const -> char & { return (**m_ptr).front(); }

		NODISCARD auto back() const -> char & { return (**m_ptr).back(); }

	public:
		NODISCARD auto begin() -> iterator { return (**m_ptr).begin(); }

		NODISCARD auto begin() const -> const_iterator { return (**m_ptr).begin(); }

		NODISCARD auto cbegin() const -> const_iterator { return (**m_ptr).cbegin(); }

		NODISCARD auto end() -> iterator { return (**m_ptr).end(); }

		NODISCARD auto end() const -> const_iterator { return (**m_ptr).end(); }

		NODISCARD auto cend() const -> const_iterator { return (**m_ptr).cend(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ism
{
	template <class T> NODISCARD STR repr(Handle<T> const & o) noexcept
	{
		TYPE t{ typeof(o) };
		return t && t->tp_repr ? t->tp_repr(o) : nullptr;
	}
}

#endif // !_ISM_STRING_OBJECT_HPP_
