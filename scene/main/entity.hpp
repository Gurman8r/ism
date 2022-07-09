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

		EntityID m_entity_id{ entt::null };

		Entity();

	public:
		virtual ~Entity() override;

		EntityID get_entity_id() const noexcept
		{
			return m_entity_id;
		}

		template <class C, class ... Args
		> C & add_component(Args && ... args) noexcept
		{
			ASSERT(m_tree);
			C & c{ m_tree->m_ecs.emplace<C>(m_entity_id, FWD(args)...) };
			m_tree->on_component_added<C>(*this, c);
			return m_tree;
		}

		template <class ... C
		> decltype(auto) get_component()
		{
			return VALIDATE(m_tree)->m_ecs.get<C...>(m_entity_id);
		}

		template <class ... C
		> bool has_component() const
		{
			return VALIDATE(m_tree)->m_ecs.has<C...>(m_entity_id);
		}

		template <class ... C
		> void remove_component()
		{
			VALIDATE(m_tree)->m_ecs.remove<C...>(m_entity_id);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_ENTITY_HPP_
