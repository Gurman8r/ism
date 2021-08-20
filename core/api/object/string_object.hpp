#ifndef _ISM_STRING_OBJECT_HPP_
#define _ISM_STRING_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// string
namespace ism
{
	// string object
	class ISM_API StringObject : public Object
	{
		ISM_OBJECT(StringObject, Object);

	protected:
		static void _bind_methods();

	public:
		String m_string{};

		using storage_type = decltype(m_string);

		using iterator = storage_type::iterator;

		using const_iterator = storage_type::const_iterator;

		using allocator_type = storage_type::allocator_type;

		NODISCARD auto & operator*() const { return const_cast<storage_type &>(m_string); }

		NODISCARD auto * operator->() const { return const_cast<storage_type *>(&m_string); }

		StringObject(allocator_type al = {}) noexcept : Object{ get_class_static() }, m_string{ al } {}

		StringObject(storage_type const & v, allocator_type al = {}) : Object{ get_class_static() }, m_string{ v, al } {}

		StringObject(storage_type && v, allocator_type al = {}) noexcept : Object{ get_class_static() }, m_string{ std::move(v), al } {}

		StringObject(cstring v, allocator_type al = {}) : Object{ get_class_static() }, m_string{ v, al } {}

		StringObject(cstring v, size_t n, allocator_type al = {}) : Object{ get_class_static() }, m_string{ v, n, al } {}

		StringObject(StringName const & v, allocator_type al = {}) : StringObject{ v.string(), al } {}

		StringObject(StringName && v, allocator_type al = {}) noexcept : StringObject{ std::move(v).string(), al } {}

		StringObject(std::initializer_list<char> init, allocator_type al = {}) : StringObject{ storage_type{ init.begin(), init.end() }, al } {}

		template <class T> StringObject(Handle<T> const & o, allocator_type al = {}) : StringObject{ al }
		{
			if constexpr (std::is_same_v<T, StringObject>)
			{
				m_string = (storage_type)o;
			}
			else if (STR::check_(o))
			{
				m_string = (storage_type)STR(o);
			}
			else if (TYPE t{ typeof(o) }; t->tp_str)
			{
				m_string = (storage_type)t->tp_str(o);
			}
		}
	};

	// string delete
	template <> struct DefaultDelete<StringObject> : DefaultDelete<Object> {};

	// string check
#define ISM_CHECK_STR(o) (ism::typeof(o).has_feature(ism::TypeFlags_Str_Subclass))

	// string handle
	template <> class Handle<StringObject> : public Ref<StringObject>
	{
		ISM_HANDLE(StringObject, ISM_CHECK_STR);

	public:
		using storage_type = value_type::storage_type;

		using iterator = value_type::iterator;

		using const_iterator = value_type::const_iterator;

		using allocator_type = value_type::allocator_type;

		void reserve(size_t count) { (**m_ptr).reserve(count); }

		void resize(size_t count) { (**m_ptr).resize(count); }

		NODISCARD operator storage_type() const { return storage_type(**m_ptr); }

		NODISCARD auto string() const & -> storage_type const & { return **m_ptr; }

		NODISCARD auto string() & -> storage_type & { return **m_ptr; }

		NODISCARD auto c_str() const { return (**m_ptr).c_str(); }

		NODISCARD auto data() const { return (**m_ptr).data(); }

		NODISCARD bool empty() const { return (**m_ptr).empty(); }

		NODISCARD auto size() const { return (**m_ptr).size(); }

		NODISCARD auto front() const -> char & { return (**m_ptr).front(); }

		NODISCARD auto back() const -> char & { return (**m_ptr).back(); }

		NODISCARD auto begin() -> iterator { return (**m_ptr).begin(); }

		NODISCARD auto begin() const -> const_iterator { return (**m_ptr).begin(); }

		NODISCARD auto cbegin() const -> const_iterator { return (**m_ptr).cbegin(); }

		NODISCARD auto end() -> iterator { return (**m_ptr).end(); }

		NODISCARD auto end() const -> const_iterator { return (**m_ptr).end(); }

		NODISCARD auto cend() const -> const_iterator { return (**m_ptr).cend(); }
	};
}

// functions
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> NODISCARD STR repr(Handle<T> const & o) noexcept
	{
		if (!o) { return nullptr; }

		TYPE t{ typeof(o) };

		return t->tp_repr ? t->tp_repr(o) : nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// "__x__"
	inline bool is_dunder_name(OBJ name)
	{
		if (!STR::check_(name)) { return false; }

		String const & s{ STR(name).string() };

		size_t const n{ s.size() };

		return (n >= 5) && (s[0] == '_') && (s[1] == '_') && (s[n - 2] == '_') && (s[n - 1] == '_');
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_STRING_OBJECT_HPP_
