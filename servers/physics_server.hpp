#ifndef _ISM_PHYSICS_SERVER_HPP_
#define _ISM_PHYSICS_SERVER_HPP_

// PLACEHOLDER

#include <core/object/class.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// audio server - PLACEHOLDER
	class PhysicsServer : public Object
	{
		DEFINE_CLASS(PhysicsServer, Object);

		static PhysicsServer * __singleton;

	public:
		PhysicsServer();
		virtual ~PhysicsServer() override;
		FORCE_INLINE static PhysicsServer * get_singleton() noexcept { return __singleton; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using PS = PhysicsServer;

	SINGLETON_WRAPPER(PS, get_physics_server);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_PHYSICS_SERVER_HPP_
