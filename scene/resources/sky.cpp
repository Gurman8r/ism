#include <scene/resources/sky.hpp>

using namespace ism;

ISM_OBJECT_IMPLEMENTATION(Sky, t, "sky")
{
	t.tp_base = typeof<Resource>();
}

Sky::~Sky()
{
}

Sky::Sky() : Resource{}
{
}