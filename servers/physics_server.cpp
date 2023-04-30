#include <servers/physics_server.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(PhysicsServer, t) {}

	PhysicsServer * PhysicsServer::__singleton{};

	PhysicsServer::PhysicsServer() { SINGLETON_CTOR(); }

	PhysicsServer::~PhysicsServer() { SINGLETON_DTOR(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}