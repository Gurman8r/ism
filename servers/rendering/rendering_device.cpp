#include <servers/rendering/rendering_device.hpp>

using namespace ism;

VAR_IMPL(RenderingDevice::singleton) {};

OBJ_IMPL(RenderingDevice, t, TypeFlags_BaseType | TypeFlags_IsAbstract) {}