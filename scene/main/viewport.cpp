#include <scene/main/viewport.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBEDED_CLASS(Viewport, t)
{
	CLASS_DEF(Viewport, t)
	{
		return t
			.def("_process", &Viewport::process)
			;
	};
}

Viewport::Viewport() {}

Viewport::~Viewport() {}

void Viewport::process(Duration const & dt)
{
	Node::process(dt);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBEDED_CLASS(SubViewport, t)
{
	CLASS_DEF(SubViewport, t)
	{
		return t
			.def("_process", &SubViewport::process)
			;
	};
}

SubViewport::SubViewport() {}

SubViewport::~SubViewport() {}

void SubViewport::process(Duration const & dt)
{
	Node::process(dt);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */