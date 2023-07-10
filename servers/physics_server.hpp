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
		OBJECT_CLASS(PhysicsServer, Object);

		SINGLETON_CLASS(PhysicsServer);

	public:
		PhysicsServer();
		virtual ~PhysicsServer() override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using PS = PhysicsServer;

	SINGLETON_WRAPPER(PhysicsServer, physics_server);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_PHYSICS_SERVER_HPP_
