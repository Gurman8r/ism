#include <servers/physics_server.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(PhysicsServer, t) {}

	PhysicsServer * PhysicsServer::__singleton{};

	PhysicsServer::CreateFunc PhysicsServer::__create_func{ []() { return memnew(PhysicsServer); } };

	PhysicsServer::PhysicsServer()
	{
		__singleton = this;
	}

	PhysicsServer::~PhysicsServer()
	{
	}

	PhysicsServer * PhysicsServer::create()
	{
		return VALIDATE(__create_func)();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}