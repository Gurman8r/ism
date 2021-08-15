#ifndef _ISM_SCRIPT_SERVER_HPP_
#define _ISM_SCRIPT_SERVER_HPP_

#include <core/api/class.hpp>

namespace ism
{
	class ISM_API ScriptServer : public Super
	{
		ISM_SUPER(ScriptServer, Super);

		static ScriptServer * singleton;

	public:
		virtual ~ScriptServer();

		static ScriptServer * get_singleton() { return singleton; }
	};
}

#endif // !_ISM_SCRIPT_SERVER_HPP_
