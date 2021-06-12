#ifndef _ISM_INT_OBJECT_HPP_
#define _ISM_INT_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// bool
#define Core_True		(ism::api::INT(&ism::api::IntObject::g_true))
#define Core_False		(ism::api::INT(&ism::api::IntObject::g_false))
#define Core_Bool(b)	((b) ? Core_True : Core_False)

// int object
class NODISCARD ISM_API ism::api::IntObject : public BaseObject
{
	ISM_OBJECT(IntObject, BaseObject);

protected:
	static void _bind_methods(TypeObject & t);

public:
	int64_t m_int{};

	using storage_type = decltype(m_int);

	template <class T = storage_type, class = std::enable_if_t<std::is_integral_v<T>>
	> explicit IntObject(TypeObject const * t, T v) : base_type{ t }, m_int{ static_cast<storage_type>(v) } {}

	template <class T = storage_type, class = std::enable_if_t<std::is_integral_v<T>>
	> IntObject(T v) : base_type{ &ob_type_static }, m_int{ static_cast<storage_type>(v) } {}

	template <class T = storage_type, class = std::enable_if_t<std::is_integral_v<T>>
	> IntObject & operator=(T const v) { m_int = static_cast<storage_type>(v); return (*this); }

	template <class T = storage_type, class = std::enable_if_t<std::is_integral_v<T>>
	> NODISCARD operator T() const & { return static_cast<T>(m_int); }

	NODISCARD auto & operator*() const { return const_cast<storage_type &>(m_int); }

public:
	static IntObject g_true; // global true

	static IntObject g_false; // global false
};

// int deleter
template <> struct ism::DefaultDelete<ism::api::IntObject> : DefaultDelete<ism::api::BaseObject> {};

// int handle
template <> class ism::api::Handle<ism::api::IntObject> : public BaseHandle<IntObject>
{
	ISM_HANDLE(IntObject);

public:
	Handle() = default;

	~Handle() = default;

	using storage_type = IntObject::storage_type;

	template <class T, class = std::enable_if_t<std::is_integral_v<T>>
	> Handle(T v) { revalue(v); }

	template <class T, class = std::enable_if_t<std::is_integral_v<T>>
	> operator T () const { return (T)(**m_ref); }
};

#endif // !_ISM_INT_OBJECT_HPP_
