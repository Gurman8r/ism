#ifndef _ISM_PROPERTY_OBJECT_HPP_
#define _ISM_PROPERTY_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// property
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace detail
	{
		struct NODISCARD property_record
		{
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class NODISCARD ISM_API CoreProperty : public CoreObject
	{
		ISM_OBJECT(CoreProperty, CoreObject);

	protected:
		static void _bind_methods(CoreType & t);

	public:
		OBJECT m_get{};
		OBJECT m_set{};
		void * m_closure{};
		String m_name{}, m_doc{};

	public:
		CoreProperty(OBJECT fget, OBJECT fset, void * closure = nullptr) : base_type{ &ob_type_static }
		{
			m_get = fget;
			m_set = fset;
			m_closure = closure;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct DefaultDelete<CoreProperty> : DefaultDelete<CoreObject> {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> class Handle<CoreProperty> : public BaseHandle<CoreProperty>
	{
		ISM_HANDLE(CoreProperty);

	public:
		Handle() = default;

		~Handle() = default;

		NODISCARD OBJECT get(OBJECT o) const { return (*m_ref).m_get(o); }

		Error set(OBJECT o, OBJECT v) const { return (*m_ref).m_set(o, v), Error_None; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_PROPERTY_OBJECT_HPP_
