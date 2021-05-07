#ifndef _ISM_API_TYPES_INT_OBJECT_HPP_
#define _ISM_API_TYPES_INT_OBJECT_HPP_

#include <core/api/types/type_object.hpp>

// int
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class NODISCARD ISM_API CoreInt : public CoreObject
	{
		ISM_OBJECT_DEFAULT(CoreInt, CoreObject);

	protected:
		static void _bind_class(CoreType & t);

	private:
		int64_t m_data{};

	public:
		using storage_type = decltype(m_data);
		static CoreInt g_true; // global true
		static CoreInt g_false; // global false

		NODISCARD operator storage_type * () const { return const_cast<storage_type *>(&m_data); }

		template <class T = storage_type, class = std::enable_if_t<std::is_integral_v<T>>
		> explicit CoreInt(TYPE const & t, T v) : base_type{ t }, m_data{ static_cast<storage_type>(v) } {}

		template <class T = storage_type, class = std::enable_if_t<std::is_integral_v<T>>
		> CoreInt(T v) : base_type{ type_static() }, m_data{ static_cast<storage_type>(v) } {}

		template <class T = storage_type, class = std::enable_if_t<std::is_integral_v<T>>
		> CoreInt & operator=(T const v) { m_data = static_cast<storage_type>(v); return (*this); }

		template <class T = storage_type, class = std::enable_if_t<std::is_integral_v<T>>
		> NODISCARD operator T() const & { return static_cast<T>(m_data); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_API_TYPES_INT_OBJECT_HPP_
