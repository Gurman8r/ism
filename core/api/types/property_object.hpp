#ifndef _ISM_API_TYPES_PROPERTY_OBJECT_HPP_
#define _ISM_API_TYPES_PROPERTY_OBJECT_HPP_

#include <core/api/types/type_object.hpp>

// property
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace detail
	{
		struct property_record
		{
			OBJECT get{};
			OBJECT set{};
			void * closure{};
			String name{}, doc{};
		};
	}

	class ISM_API CoreProperty : public CoreObject
	{
		ISM_OBJECT_DEFAULT(CoreProperty, CoreObject);

	protected:
		static void _bind_class(CoreType & t);

	private:
		detail::property_record m_data{};

		using storage_type = decltype(m_data);

	public:
		CoreProperty(OBJECT fget, OBJECT fset, void * closure, String const & doc = "") : base_type{ type_static() }
		{
			m_data = { fget, fset, closure, "", doc };
		}

		NODISCARD operator storage_type * () const { return const_cast<storage_type *>(&m_data); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_API_TYPES_PROPERTY_OBJECT_HPP_
