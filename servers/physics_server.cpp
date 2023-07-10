#include <servers/physics_server.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(PhysicsServer, t) {}

	SINGLETON_EMBED(PhysicsServer);

	PhysicsServer::PhysicsServer() { SINGLETON_CTOR(); }

	PhysicsServer::~PhysicsServer() { SINGLETON_DTOR(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}