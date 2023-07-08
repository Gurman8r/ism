#ifndef _ISM_VERSION_HPP_
#define _ISM_VERSION_HPP_

#include "./preprocessor.hpp"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef VERSION_NAME
#error "undefined version name"
#endif

#ifndef VERSION_MAJOR
#error "undefined version major"
#endif

#ifndef VERSION_MINOR
#error "undefined version minor"
#endif

#ifndef VERSION_PATCH
#error "undefined version patch"
#endif

#ifndef VERSION_STATUS
#error "undefined version status"
#endif

#ifndef VERSION_BUILD
#error "undefined version build"
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// version branch
#define VERSION_BRANCH \
		XSTR(VERSION_MAJOR) "." XSTR(VERSION_MINOR)

// version number
#define VERSION_NUMBER \
		VERSION_BRANCH "." XSTR(VERSION_PATCH)

// version hex
#define VERSION_HEX \
		(0x10000 * VERSION_MAJOR + 0x100 * VERSION_MINOR + VERSION_PATCH)

// version full config
#define VERSION_FULL_CONFIG \
		VERSION_NUMBER "." VERSION_STATUS

// version full build
#define VERSION_FULL_BUILD \
		VERSION_FULL_CONFIG "." VERSION_BUILD

// version full name
#define VERSION_FULL_NAME \
		VERSION_NAME " v" VERSION_FULL_BUILD

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ISM_VERSION_HPP_
