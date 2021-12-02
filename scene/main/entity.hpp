#ifndef _ISM_ENTITY_HPP_
#define _ISM_ENTITY_HPP_

#include <scene/main/node.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Entity : public Node
	{
		OBJECT_COMMON(Entity, Node);

	protected:
		friend class SceneTree;

		EntityID m_entity_id{ entt::null }; // entity

		Entity();

	public:
		virtual ~Entity() override;

	public:
		NODISCARD operator EntityID() const noexcept { return m_entity_id; }

		NODISCARD auto get_entity_id() const noexcept -> EntityID { return m_entity_id; }

	public:
		template <class Component, class ... Args
		> Component & add_component(Args && ... args) noexcept
		{
			auto & tree{ *VALIDATE(get_tree()) };
			Component & c{ tree.m_entt.emplace<Component>(m_entity_id, FWD(args)...) };
			tree.on_component_added<Component>(*this, c);
			return c;
		}

		template <class ... Component
		> NODISCARD decltype(auto) get_component()
		{
			auto & tree{ *VALIDATE(get_tree()) };
			return tree.m_entt.get<Component...>(m_entity_id);
		}

		template <class ... Component
		> NODISCARD bool has_component() const
		{
			auto & tree{ *VALIDATE(get_tree()) };
			return tree.m_entt.has<Component...>(m_entity_id);
		}

		template <class ... Component
		> void remove_component()
		{
			auto & tree{ *VALIDATE(get_tree()) };
			return tree.m_entt.remove<Component...>(m_entity_id);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_ENTITY_HPP_
