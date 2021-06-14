#ifndef _ISM_DISPLAY_SERVER_HPP_
#define _ISM_DISPLAY_SERVER_HPP_

#include <core/api/modsupport.hpp>

namespace ism
{
	class ISM_API DisplayServer : public Super
	{
		ISM_SUPER(DisplayServer, Super);

	public:
		virtual ~DisplayServer() override;

		DEFAULT_COPYABLE_MOVABLE(DisplayServer);
	};
}

#endif // !_ISM_DISPLAY_SERVER_HPP_
