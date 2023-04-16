#include <servers/physics_server.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	PhysicsServer * PhysicsServer::__singleton{};

	EMBED_CLASS(PhysicsServer, t) {}

	PhysicsServer::PhysicsServer() { __singleton = this; }

	PhysicsServer::~PhysicsServer() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}