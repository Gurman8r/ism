#ifndef _ISM_API_TYPES_FLOAT_OBJECT_HPP_
#define _ISM_API_TYPES_FLOAT_OBJECT_HPP_

#include <core/api/types/type_object.hpp>

// float
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class NODISCARD ISM_API CoreFloat : public CoreObject
	{
		ISM_OBJECT_DEFAULT(CoreFloat, CoreObject);

	protected:
		static void _bind_class(CoreType & t);

	public:
		using storage_type = double_t;

		template <class T = storage_type, class = std::enable_if_t<std::is_floating_point_v<T>>
		> explicit CoreFloat(TYPE const & t, T v) : base_type{ t }, m_data{ static_cast<storage_type>(v) } {}

		template <class T = storage_type, class = std::enable_if_t<std::is_floating_point_v<T>>
		> CoreFloat(T v) : base_type{ &ob_type_static }, m_data{ static_cast<storage_type>(v) } {}

		template <class T = storage_type, class = std::enable_if_t<std::is_floating_point_v<T>>
		> CoreFloat & operator=(T const v) { m_data = static_cast<storage_type>(v); return (*this); }

		template <class T = storage_type, class = std::enable_if_t<std::is_floating_point_v<T>>
		> NODISCARD operator T() const & { return static_cast<T>(m_data); }

		NODISCARD operator storage_type * () const { return const_cast<storage_type *>(&m_data); }

	private:
		storage_type m_data{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_API_TYPES_FLOAT_OBJECT_HPP_
