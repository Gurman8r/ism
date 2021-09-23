#ifndef _ISM_REGISTER_CORE_TYPES_HPP_
#define _ISM_REGISTER_CORE_TYPES_HPP_

#include <core/language_features.hpp>

namespace ism
{
	API_FUNC(void) register_core_types();

	API_FUNC(void) register_core_driver_types();

	API_FUNC(void) register_core_settings();

	API_FUNC(void) register_core_singletons();

	API_FUNC(void) unregister_core_driver_types();

	API_FUNC(void) unregister_core_types();
}

#endif // !_ISM_REGISTER_CORE_TYPES_HPP_
