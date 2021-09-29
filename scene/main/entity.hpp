#ifndef _ISM_ENTITY_HPP_
#define _ISM_ENTITY_HPP_

#include <scene/main/node.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Entity : public Node
	{
		OBJECT_COMMON(Entity, Node);

		EntityID m_entity{}; // entity

	public:
		virtual ~Entity();

	protected:
		explicit Entity(SceneTree * tree, Node * parent);

		explicit Entity(SceneTree * tree);

		explicit Entity(Node * parent);

	public:
		NODISCARD operator EntityID() const noexcept { return m_entity; }

		NODISCARD auto get_entity_id() const noexcept -> EntityID { return m_entity; }

		template <class Component, class ... Args
		> Component & add_component(Args && ... args) noexcept
		{
			Component & c{ m_tree->m_reg.emplace<Component>(m_entity, FWD(args)...) };
			m_tree->on_component_added<Component>(*this, c);
			return c;
		}

		template <class ... Component
		> NODISCARD decltype(auto) get_component()
		{
			return m_tree->m_reg.get<Component...>(m_entity);
		}

		template <class ... Component
		> NODISCARD bool has_component() const
		{
			return m_tree->m_reg.has<Component...>(m_entity);
		}

		template <class ... Component
		> void remove_component()
		{
			m_tree->m_reg.remove<Component...>(m_entity);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_ENTITY_HPP_
