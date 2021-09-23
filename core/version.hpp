#ifndef _ISM_VERSION_HPP_
#define _ISM_VERSION_HPP_

#include <core/preprocessor.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef ISM_VERSION_MAJOR
#define ISM_VERSION_MAJOR 0
#endif

#ifndef ISM_VERSION_MINOR
#define ISM_VERSION_MINOR 0
#endif

#ifndef ISM_VERSION_PATCH
#define ISM_VERSION_PATCH 0
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ISM_VERSION_BRANCH "" XSTR(ISM_VERSION_MAJOR) "." XSTR(ISM_VERSION_MINOR)

#define ISM_VERSION_NUMBER "" ISM_VERSION_BRANCH "." XSTR(ISM_VERSION_PATCH)

#define ISM_VERSION_FULL_CONFIG "" ISM_VERSION_NUMBER

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define __OS_IMPL__		__os_impl__

#define OS_IMPL(user)	ism::OS & __OS_IMPL__(user)

#define OS_LAUNCH(user)	static ism::OS & ANON{ __OS_IMPL__(user) }

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ISM_VERSION_HPP_
