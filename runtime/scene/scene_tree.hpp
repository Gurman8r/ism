#ifndef _ISM_SCENE_TREE_HPP_
#define _ISM_SCENE_TREE_HPP_

#include <core/os/main_loop.hpp>
#include <core/templates/duration.hpp>
#include <runtime/window/window.hpp>

// scene
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API SceneTree : public MainLoop
	{
		OBJECT_COMMON(SceneTree, MainLoop);

	public:
		virtual ~SceneTree() override;

		explicit SceneTree(String const & name = "New Scene");

	public:
		virtual void initialize() override;

		virtual bool process(Duration const & delta_time) override;

		virtual void finalize() override;

	public:
		void on_runtime_updateprocess(Duration const & delta_time);

	protected:
		template <class T> void on_component_added(Node &, T &) {}

	public:
		NODISCARD auto get_registry() const noexcept -> EntityRegistry * { return const_cast<EntityRegistry *>(&m_reg); }

		NODISCARD auto get_window() const noexcept -> Window * { return m_window; }
		
		NODISCARD auto get_root() const noexcept -> Node * { return m_root; }

		NODISCARD auto get_name() const noexcept -> String const & { return m_name; }

		void set_name(String const & value) noexcept { if (m_name != value) { m_name = value; } }

	private:
		friend class Node;

		String			m_name		{};
		Window *		m_window	{};
		Node *			m_root		{};
		EntityRegistry	m_reg		{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_SCENE_TREE_HPP_
