#ifndef _ISM_ENTITY_HPP_
#define _ISM_ENTITY_HPP_

#include <scene/main/node.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Entity : public Node
	{
		OBJECT_COMMON(Entity, Node);

		entt::entity m_entity_id{}; // entity

	protected:
		Entity(SceneTree * scene = nullptr, Node * parent = nullptr) noexcept;

	public:
		NODISCARD operator entt::entity() const noexcept { return m_entity_id; }

		NODISCARD auto get_entity_id() const noexcept -> entt::entity { return m_entity_id; }

		template <class Component, class ... Args
		> Component & add_component(Args && ... args) noexcept
		{
			Component & c{ m_scene->m_entt.emplace<Component>(m_entity_id, FWD(args)...) };
			m_scene->on_component_added<Component>(*this, c);
			return c;
		}

		template <class ... Component
		> NODISCARD decltype(auto) get_component()
		{
			return m_scene->m_entt.get<Component...>(m_entity_id);
		}

		template <class ... Component
		> NODISCARD bool has_component() const
		{
			return m_scene->m_entt.has<Component...>(m_entity_id);
		}

		template <class ... Component
		> void remove_component()
		{
			m_scene->m_entt.remove<Component...>(m_entity_id);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_ENTITY_HPP_
