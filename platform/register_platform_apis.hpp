#ifndef _ISM_REGISTER_PLATFORM_APIS_HPP_
#define _ISM_REGISTER_PLATFORM_APIS_HPP_

#include <core/language_features.hpp>

namespace ism
{
	API_FUNC(void) register_platform_apis();

	API_FUNC(void) unregister_platform_apis();
}

#endif // !_ISM_REGISTER_PLATFORM_APIS_HPP_
