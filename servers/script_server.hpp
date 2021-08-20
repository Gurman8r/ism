#ifndef _ISM_SCRIPT_SERVER_HPP_
#define _ISM_SCRIPT_SERVER_HPP_

#include <core/api/class.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// script server
	class ISM_API ScriptServer
	{
		friend class Main;

		static ScriptServer * singleton;

		ScriptServer();

	public:
		virtual ~ScriptServer();

		static ScriptServer * get_singleton() { return singleton; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD inline ScriptServer * get_script_server() { return ScriptServer::get_singleton(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_SCRIPT_SERVER_HPP_
