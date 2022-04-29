#ifndef _ISM_TEXT_SERVER_HPP_
#define _ISM_TEXT_SERVER_HPP_

#include <core/object/detail/class.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API TextServer : public Object
	{
		OBJECT_COMMON(TextServer, Object);

		static TextServer * singleton;

	public:
		explicit TextServer();

		virtual ~TextServer();

		NODISCARD static TextServer * get_singleton() noexcept { return singleton; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define TS (ism::TextServer::get_singleton())

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_TEXT_SERVER_HPP_
