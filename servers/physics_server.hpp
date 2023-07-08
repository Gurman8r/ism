#ifndef _ISM_PHYSICS_SERVER_HPP_
#define _ISM_PHYSICS_SERVER_HPP_

// PLACEHOLDER

#include <core/object/class.hpp>

namespace Ism
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
		SINGLETON_GETTER(PhysicsServer, __singleton);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using PS = PhysicsServer;

	SINGLETON_WRAPPER(PS, get_physics);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_PHYSICS_SERVER_HPP_
