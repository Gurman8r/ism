#include <main/performance.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(Performance, t) {}

	Performance * Performance::__singleton{};

	Performance::Performance()
	{
		__singleton = this;
	}

	Performance::~Performance()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}