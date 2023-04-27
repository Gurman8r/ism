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

	protected:
		using CreateFunc = PhysicsServer * (*)();
		static CreateFunc __create_func;
		PhysicsServer();

	public:
		virtual ~PhysicsServer() override;
		FORCE_INLINE static PhysicsServer * get_singleton() noexcept { return __singleton; }
#define PHYSICS_SERVER (ism::PhysicsServer::get_singleton())
		static PhysicsServer * create();
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using PS = PhysicsServer;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_PHYSICS_SERVER_HPP_
