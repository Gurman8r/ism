#ifndef _ISM_COMPONENT_HPP_
#define _ISM_COMPONENT_HPP_

#include <scene/main/entity.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Component : public Object
	{
		OBJECT_COMMON(Component, Object);

		Entity * m_entity{};

	protected:
		Component(Entity * entity) noexcept;

	public:
		virtual ~Component() noexcept override;

		Entity * get_entity() const noexcept { return m_entity; }

		template <class T, class ... Args
		> T & add_component(Args && ... args) noexcept { return VALIDATE(m_entity)->add_component<T>(FWD(args)...); }

		template <class ... T
		> decltype(auto) get_component() noexcept { return VALIDATE(m_entity)->get_component<T...>(); }

		template <class ... T
		> bool has_component() const noexcept { return VALIDATE(m_entity)->has_component<T...>(); }

		template <class ... T
		> void remove_component() noexcept { VALIDATE(m_entity)->remove_component<T...>(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_COMPONENT_HPP_
