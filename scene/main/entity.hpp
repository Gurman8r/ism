#ifndef _ISM_ENTITY_HPP_
#define _ISM_ENTITY_HPP_

#include <scene/main/scene_tree.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Entity : public Node
	{
		ISM_SUPER(Entity, Node);

	public:
		virtual ~Entity() override;

		explicit Entity(Node * parent, SceneTree * tree) noexcept : Node{ parent, tree }, m_ent{ get_registry()->create() } {}

		explicit Entity(SceneTree * tree) noexcept : Node{ tree }, m_ent{ get_registry()->create() } {}

		explicit Entity(Node * parent) noexcept : Node{ parent }, m_ent{ get_registry()->create() } {}

		NON_COPYABLE(Entity);

		MOVABLE(Entity);

	public:
		template <class Component, class ... Args
		> Component & add_component(Args && ... args) noexcept
		{
			Component & cpt{ get_registry()->emplace<Component>(m_ent, FWD(args)...) };
			m_tree->on_component_added<Component>(*this, c);
			return cpt;
		}

		template <class ... Component
		> NODISCARD decltype(auto) get_component()
		{
			return get_registry()->get<Component...>(m_ent);
		}

		template <class ... Component
		> NODISCARD bool has_component() const
		{
			return get_registry()->has<Component...>(m_ent);
		}

		template <class ... Component
		> void remove_component()
		{
			get_registry()->remove<Component...>(m_ent);
		}

	public:
		NODISCARD operator entt::entity() const noexcept { return m_ent; }

		NODISCARD auto get_entity_id() const noexcept -> entt::entity { return m_ent; }

		NODISCARD auto get_registry() const noexcept -> entt::registry * { return CHECK(get_tree())->get_registry(); }

		NODISCARD bool is_entity_valid() const noexcept { return get_registry()->valid(m_ent); }

	protected:
		entt::entity m_ent{ entt::null }; // entity
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_ENTITY_HPP_
