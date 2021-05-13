#ifndef _ISM_INT_OBJECT_HPP_
#define _ISM_INT_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// bool
#define Core_True		(ism::INT(&ism::CoreInt::g_true))
#define Core_False		(ism::INT(&ism::CoreInt::g_false))
#define Core_Bool(b)	((b) ? Core_True : Core_False)

// int
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class NODISCARD ISM_API CoreInt : public CoreObject
	{
		ISM_OBJECT(CoreInt, CoreObject);

	protected:
		static void _bind_methods(CoreType & t);

	protected:
		int64_t m_int{};

	public:
		using storage_type = decltype(m_int);

		template <class T = storage_type, class = std::enable_if_t<std::is_integral_v<T>>
		> explicit CoreInt(CoreType const * t, T v) : base_type{ t }, m_int{ static_cast<storage_type>(v) } {}

		template <class T = storage_type, class = std::enable_if_t<std::is_integral_v<T>>
		> CoreInt(T v) : base_type{ &ob_type_static }, m_int{ static_cast<storage_type>(v) } {}

		template <class T = storage_type, class = std::enable_if_t<std::is_integral_v<T>>
		> CoreInt & operator=(T const v) { m_int = static_cast<storage_type>(v); return (*this); }

		template <class T = storage_type, class = std::enable_if_t<std::is_integral_v<T>>
		> NODISCARD operator T() const & { return static_cast<T>(m_int); }

		NODISCARD auto & operator*() const { return const_cast<storage_type &>(m_int); }

	public:
		static CoreInt g_true; // global true

		static CoreInt g_false; // global false
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// INT
	template <> class Handle<CoreInt> : public BaseHandle<CoreInt>
	{
		ISM_HANDLE(CoreInt);

	public:
		Handle() = default;

		~Handle() = default;

		using storage_type = CoreInt::storage_type;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_INT_OBJECT_HPP_
