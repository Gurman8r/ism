#ifndef _ISM_COMPONENT_HPP_
#define _ISM_COMPONENT_HPP_

#include <core/object/class.hpp>

namespace Ism
{
	class Entity;
	class TransformComponent;

	class ISM_API Component : public Object
	{
		OBJECT_CLASS(Component, Object);

		Entity * m_entity{};

	public:
		Component();
		virtual ~Component() override;

		template <class C, class ... Args
		> C & add_component(Args && ... args) noexcept { return VALIDATE(m_entity)->add_component<C>(FWD(args)...); }

		template <class ... C
		> decltype(auto) get_component() { return VALIDATE(m_entity)->get_component<C...>(); }

		template <class ... C
		> bool has_component() const { return VALIDATE(m_entity)->has_component<C...>(); }

		template <class ... C
		> void remove_component() { VALIDATE(m_entity)->remove_component<C...>(); }
	};
}

#endif // !_ISM_COMPONENT_HPP_
