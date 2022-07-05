#ifndef _ISM_ENTITY_HPP_
#define _ISM_ENTITY_HPP_

#include <scene/main/scene_tree.hpp>

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

		operator EntityID() const noexcept { return m_entity_id; }

		auto get_entity_id() const noexcept -> EntityID { return m_entity_id; }

		template <class C, class ... Args
		> C & add_component(Args && ... args) noexcept
		{
			auto * t{ VALIDATE(get_tree()) };
			C & c{ t->get_ecs().emplace<C>(m_entity_id, FWD(args)...) };
			t->on_component_added<C>(*this, c);
			return c;
		}

		template <class ... C
		> decltype(auto) get_component()
		{
			return VALIDATE(get_tree())->get_ecs().get<C...>(m_entity_id);
		}

		template <class ... C
		> bool has_component() const
		{
			return VALIDATE(get_tree())->get_ecs().has<C...>(m_entity_id);
		}

		template <class ... C
		> void remove_component()
		{
			VALIDATE(get_tree())->get_ecs().remove<C...>(m_entity_id);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_ENTITY_HPP_
