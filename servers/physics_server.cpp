#include <servers/physics_server.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(PhysicsServer, t) {}

	PhysicsServer * PhysicsServer::__singleton{};

	PhysicsServer::PhysicsServer() { SINGLETON_CTOR(__singleton, this); }

	PhysicsServer::~PhysicsServer() { SINGLETON_DTOR(__singleton, this); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}