#ifndef _ISM_BEHAVIOR_HPP_
#define _ISM_BEHAVIOR_HPP_

#include <scene/components/component.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Behavior : public Component
	{
		OBJECT_COMMON(Behavior, Component);

		bool m_is_enabled{ true };

	protected:
		Behavior(Entity * entity) noexcept;

	public:
		virtual ~Behavior() noexcept override;

		bool is_enabled() const noexcept { return m_is_enabled; }

		void set_enabled(bool value) noexcept { m_is_enabled = value; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_BEHAVIOR_HPP_
