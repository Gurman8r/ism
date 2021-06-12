#ifndef _ISM_DISPLAY_SERVER_HPP_
#define _ISM_DISPLAY_SERVER_HPP_

#include <core/api/super.hpp>

namespace ism
{
	class ISM_API DisplayServer : public Super
	{
		ISM_SUPER_CLASS(DisplayServer, Super);

	public:
		virtual ~DisplayServer() override;

		DEFAULT_COPY_AND_MOVE_CONSTRUCTABLE(DisplayServer);
	};
}

#endif // !_ISM_DISPLAY_SERVER_HPP_
