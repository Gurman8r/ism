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

	public:
		String m_string{};

		using storage_type = decltype(m_string);

		using iterator = storage_type::iterator;

		using const_iterator = storage_type::const_iterator;

		NODISCARD auto & operator*() const { return const_cast<storage_type &>(m_string); }

		NODISCARD auto * operator->() const { return const_cast<storage_type *>(&m_string); }

		StringObject() noexcept : Object{} {}

		StringObject(storage_type const & value) : StringObject{} { m_string = value; }

		StringObject(storage_type && value) noexcept : StringObject{} { m_string = std::move(value); }

		StringObject(cstring value) : StringObject{} { m_string = value; }

		StringObject(cstring value, size_t n) : StringObject{} { m_string = { value, n }; }

		StringObject(StringName const & value) : StringObject{ value.string() } {}

		StringObject(StringName && value) noexcept : StringObject{ std::move(value).string() } {}

		StringObject(std::initializer_list<char> init) : StringObject{} { m_string = init; }

		template <class T> StringObject(Handle<T> const & value) : StringObject{}
		{
			if constexpr (std::is_same_v<T, StringObject>)
			{
				m_string = (storage_type)value;
			}
			else if (STR::check_(value))
			{
				m_string = (storage_type)STR(value);
			}
			else if (TYPE t{ typeof(value) }; t->tp_str)
			{
				m_string = (storage_type)t->tp_str(value);
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
		ISM_HANDLE(Handle, StringObject, ISM_CHECK_STR);

	public:
		using storage_type = value_type::storage_type;

		using iterator = value_type::iterator;

		using const_iterator = value_type::const_iterator;

		NODISCARD auto string() const & noexcept -> storage_type & { return **CHECK(m_ptr); }

		NODISCARD operator storage_type() const { return string(); }

		void reserve(size_t count) { string().reserve(count); }

		void resize(size_t count) { string().resize(count); }

		NODISCARD auto c_str() const { return string().c_str(); }

		NODISCARD auto data() const { return string().data(); }

		NODISCARD bool empty() const { return string().empty(); }

		NODISCARD auto size() const { return string().size(); }

		NODISCARD auto begin() -> iterator { return string().begin(); }

		NODISCARD auto begin() const -> const_iterator { return string().begin(); }

		NODISCARD auto cbegin() const -> const_iterator { return string().cbegin(); }

		NODISCARD auto end() -> iterator { return string().end(); }

		NODISCARD auto end() const -> const_iterator { return string().end(); }

		NODISCARD auto cend() const -> const_iterator { return string().cend(); }
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
