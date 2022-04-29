#ifndef _ISM_TEXT_SERVER_HPP_
#define _ISM_TEXT_SERVER_HPP_

#include <core/object/detail/class.hpp>

namespace ism
{
	class ISM_API TextServer : public Object
	{
		OBJECT_COMMON(TextServer, Object);

		static TextServer * singleton;

	public:
		explicit TextServer();

		~TextServer();
	};
}

#endif // !_ISM_TEXT_SERVER_HPP_
