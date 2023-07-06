#ifndef _ISM_VERSION_HPP_
#define _ISM_VERSION_HPP_

#include <core/preprocessor.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef _ISM_VERSION_NAME_
#error "undefined version name"
#endif

#ifndef _ISM_VERSION_MAJOR_
#error "undefined version major"
#endif

#ifndef _ISM_VERSION_MINOR_
#error "undefined version minor"
#endif

#ifndef _ISM_VERSION_PATCH_
#error "undefined version patch"
#endif

#ifndef _ISM_VERSION_STATUS_
#error "undefined version status"
#endif

#ifndef _ISM_VERSION_BUILD_
#error "undefined version build"
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define VERSION_NAME	_ISM_VERSION_NAME_
#define VERSION_MAJOR	_ISM_VERSION_MAJOR_
#define VERSION_MINOR	_ISM_VERSION_MINOR_
#define VERSION_PATCH	_ISM_VERSION_PATCH_
#define VERSION_STATUS	_ISM_VERSION_STATUS_
#define VERSION_BUILD	_ISM_VERSION_BUILD_

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
