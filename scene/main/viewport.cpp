#include <scene/main/viewport.hpp>

using namespace ism;

ISM_OBJECT_IMPLEMENTATION(Viewport, t, "viewport")
{
	t.tp_base = typeof<Node>();
}

Viewport::~Viewport() {}