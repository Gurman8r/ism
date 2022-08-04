#ifndef _ISM_INT_OBJECT_HPP_
#define _ISM_INT_OBJECT_HPP_

#include <core/object/type_object.hpp>

// bool
#define TRUE_OBJ		(ism::INT(&ism::IntObject::g_True))
#define FALSE_OBJ		(ism::INT(&ism::IntObject::g_False))
#define BOOL_OBJ(b)		((b) ? TRUE_OBJ : FALSE_OBJ)

// int
namespace ism
{
	// int object
	class ISM_API IntObject : public Object
	{
		OBJECT_CLASS(IntObject, Object);

		friend class INT;

	public:
		i64 m_int{};

		using storage_type = decltype(m_int);

		auto & operator*() const { return (storage_type &)(m_int); }

		IntObject() noexcept {}

		template <class T, class = std::enable_if_t<std::is_integral_v<T>>
		> explicit IntObject(T const value) : IntObject{} { m_int = static_cast<storage_type>(value); }

		template <class T, class = std::enable_if_t<std::is_integral_v<T>>
		> IntObject & operator=(T const value) { m_int = static_cast<storage_type>(value); return (*this); }

		template <class T, class = std::enable_if_t<std::is_integral_v<T>>
		> operator T() const & { return static_cast<T>(m_int); }

	public:
		static IntObject g_True; // global true constant

		static IntObject g_False; // global false constant
	};

	// int delete
	template <> struct DefaultDelete<IntObject> : DefaultDelete<Object> {};

	// int check
#define OBJECT_CHECK_INT(o) (ism::typeof(o).has_feature(ism::TypeFlags_Int_Subclass))

	// int ref
	class INT : public Ref<IntObject>
	{
		REF_CLASS(INT, OBJECT_CHECK_INT);

	public:
		using storage_type = value_type::storage_type;

		template <class T, class = std::enable_if_t<std::is_integral_v<T>>
		> INT(T const value) noexcept { instance(value); }

		template <class T, class = std::enable_if_t<std::is_integral_v<T>>
		> operator T () const { return (T)(**VALIDATE(m_ptr)); }

		template <class T, class = std::enable_if_t<std::is_integral_v<T>>
		> INT & operator=(T const value) noexcept
		{
			if (m_ptr) { m_ptr->m_int = value; }
			else { instance(value); }
			return (*this);
		}
	};
}

#endif // !_ISM_INT_OBJECT_HPP_
