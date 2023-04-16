#ifndef _ISM_REGISTER_DRIVER_TYPES_HPP_
#define _ISM_REGISTER_DRIVER_TYPES_HPP_

#include <core/typedefs.hpp>

namespace ism
{
	ISM_API_FUNC(void) register_core_driver_types();
	ISM_API_FUNC(void) unregister_core_driver_types();

	ISM_API_FUNC(void) register_driver_types();
	ISM_API_FUNC(void) unregister_driver_types();
}

#endif // !_ISM_REGISTER_DRIVER_TYPES_HPP_
