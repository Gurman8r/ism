#ifndef _ISM_API_TYPES_BOOL_OBJECT_HPP_
#define _ISM_API_TYPES_BOOL_OBJECT_HPP_

#include <core/api/types/int_object.hpp>

// bool
#define Core_True		(ism::INT(&ism::CoreInt::g_true))
#define Core_False		(ism::INT(&ism::CoreInt::g_false))
#define Core_Bool(b)	((b) ? Core_True : Core_False)

#endif // !_ISM_API_TYPES_BOOL_OBJECT_HPP_
