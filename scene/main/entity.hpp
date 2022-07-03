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

		template <class T, class ... Args
		> T & add_component(Args && ... args) noexcept
		{
			auto & tree{ *VALIDATE(m_tree) };
			T & c{ tree.m_entt.emplace<T>(m_entity_id, FWD(args)...) };
			tree.on_component_added<T>(*this, c);
			return c;
		}

		template <class ... T
		> decltype(auto) get_component()
		{
			auto & tree{ *VALIDATE(m_tree) };
			return tree.m_entt.get<T...>(m_entity_id);
		}

		template <class ... T
		> bool has_component() const
		{
			auto & tree{ *VALIDATE(m_tree) };
			return tree.m_entt.has<T...>(m_entity_id);
		}

		template <class ... T
		> void remove_component()
		{
			auto & tree{ *VALIDATE(m_tree) };
			tree.m_entt.remove<T...>(m_entity_id);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_ENTITY_HPP_
