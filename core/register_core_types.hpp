#ifndef _ISM_REGISTER_CORE_TYPES_HPP_
#define _ISM_REGISTER_CORE_TYPES_HPP_

#include <core/extension/native_interface.hpp>

namespace ism
{
	ISM_API_FUNC(void) register_core_types();

	ISM_API_FUNC(void) register_core_driver_types();

	ISM_API_FUNC(void) register_core_settings();

	ISM_API_FUNC(void) register_core_singletons();

	ISM_API_FUNC(void) unregister_core_driver_types();

	ISM_API_FUNC(void) unregister_core_types();
}

#endif // !_ISM_REGISTER_CORE_TYPES_HPP_
