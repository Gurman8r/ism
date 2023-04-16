#ifndef _ISM_REGISTER_SERVER_TYPES_HPP_
#define _ISM_REGISTER_SERVER_TYPES_HPP_

#include <core/typedefs.hpp>

namespace ism
{
	ISM_API_FUNC(void) register_server_types();
	ISM_API_FUNC(void) register_server_singletons();
	ISM_API_FUNC(void) unregister_server_types();
}

#endif // !_ISM_REGISTER_SERVER_TYPES_HPP_
